#include <assert.h>
#include <time.h>
#include "infrastructure/adapters/signalCapturer.hpp"
#include "application/ICapturer.hpp"
#include "application/IScreen.hpp"
#include "frameworksAndDrivers/serialPort/serialPortPsoc.hpp"
#include "frameworksAndDrivers/serialPort/serialPortMcu.hpp"

using namespace INFRA;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalCapturer::SignalCapturer() 
    : voltages_(nullptr),
    capturer_(DRV_FRAMW::SerialPortPsoc::get_instance())
{
    stateCatcher_.store(false, std::memory_order_release);
}

SignalCapturer::~SignalCapturer(){
    stop_reading();
}

void SignalCapturer::select_capturer(Capturer_t capturer){
    switch(capturer){
        case Capturer_t::PSOC:
            if(capturer_.get_Id() == APP::IdCapturer_t::SERIAL_PORT_ANY_ID)
                capturer_.close_port();
            capturer_ = DRV_FRAMW::SerialPortPsoc::get_instance();
            break;
        case Capturer_t::ANY_MCU:
            if(capturer_.get_Id() == APP::IdCapturer_t::SERIAL_PORT_PSOC_ID)
                capturer_.close_port();
            capturer_ = DRV_FRAMW::SerialPortAnyMcu::get_instance();
            break;
    }
}

void SignalCapturer::start_reading(){
    if(!catcher_.joinable()){
        stateCatcher_.store(true, std::memory_order_release);
        catcher_ = std::thread(&SignalCapturer::catch_loop, this);
    }
}

void SignalCapturer::stop_reading(){
    capturer_.close_port();
    stateCatcher_.store(false, std::memory_order_release);

    if(catcher_.joinable())
        catcher_.join();
}

void SignalCapturer::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr;
}

void SignalCapturer::associate_voltages(DOMN::VoltageSignal* voltsPtr){
    voltages_ = voltsPtr;
}

void SignalCapturer::set_time_div(double timeDiv){
    capturer_.set_data(&timeDiv);
}

std::mutex& SignalCapturer::get_mutex(){
    return mutexCatcher_;
}

void SignalCapturer::open_close_port(const char* portName){
    APP::MsgReturn_t statePort;
    if(capturer_.get_flag_serial()){
        statePort = capturer_.close_port();
    }else{
        statePort = capturer_.open_port(portName);
    }

    switch(statePort){
        case APP::MsgReturn_t::PORT_OPENED:
            screenPtr_->set_message("Serial Abierto: ", 4);
            screenPtr_->set_message(portName, 4);
            screenPtr_->set_message(".\n", 4);
            screenPtr_->update_port_state(true);
            break;
        case APP::MsgReturn_t::PORT_CLOSED:
            screenPtr_->set_message("Serial Cerrado\n", 5);
            screenPtr_->update_port_state(false);
            break;
        case APP::MsgReturn_t::DONT_NAME_PORT:
            screenPtr_->set_message("No hay nombre de puerto serie para abrir.\n", 2);
            break;
        case APP::MsgReturn_t::ERROR_IN_OPEN:
            screenPtr_->set_message("Error al abrir serial: ", 2);
            screenPtr_->set_message(portName, 2);
            screenPtr_->set_message(".\n", 2);
            break;
        case APP::MsgReturn_t::ERROR_IN_CLOSE:
            screenPtr_->set_message("Error al cerrar el puerto serie.\n", 2);
            break;
        default:
            break;
    }
}

DOMN::VoltageSignal* SignalCapturer::get_voltages_ref(){
    return voltages_;
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void SignalCapturer::catch_loop(){
    timespec t1{};
    timespec t2{};

    while(stateCatcher_.load(std::memory_order_acquire)){
        clock_gettime(CLOCK_MONOTONIC, &t1);
        capturer_.catch_data(this);

        clock_gettime(CLOCK_MONOTONIC, &t2);
        time_difference(t1, t2);
        if(screenPtr_)
            screenPtr_->set_displacement_time(t1.tv_sec, t1.tv_nsec);
    }
}

void SignalCapturer::time_difference(timespec& t1, timespec& t2){
    t1.tv_sec = t2.tv_sec - t1.tv_sec;
    t1.tv_nsec = t2.tv_nsec - t1.tv_nsec;
}
