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
        catcher_ = std::thread(&SignalCapturer::catch_loop, this);
        stateCatcher_.store(true, std::memory_order_release);
    }
}

void SignalCapturer::stop_reading(){
    stateCatcher_.store(false, std::memory_order_release);

    capturer_.close_port();

    if(catcher_.joinable())
        catcher_.join();
}

void SignalCapturer::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr;
    capturer_.associate_screen(screenPtr);
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
    if(capturer_.get_flag_serial()){
        capturer_.close_port();
    }else{
        capturer_.open_port(portName);
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
    timespec next{};
    clock_gettime(CLOCK_MONOTONIC, &next);

    while(stateCatcher_.load(std::memory_order_acquire)){
        capturer_.catch_data(this);

        if(screenPtr_ != nullptr){
            add_ns(next, screenPtr_->get_period_time_ns());
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, nullptr);
        }
    }
}

void SignalCapturer::add_ns(timespec& t, long ns){
    t.tv_nsec += ns;

    while(t.tv_nsec >= 1000000000L){
        t.tv_nsec -= 1000000000L;
        t.tv_sec++;
    }
}
