#include "infrastructure/adapters/signalCapturer.hpp"
#include "application/ICapturer.hpp"
#include "application/IScreen.hpp"

using namespace INFRA;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalCapturer::SignalCapturer(std::unique_ptr<APP::ICapturer> capturer) 
    : voltages_(nullptr),
    capturerPtr_(std::move(capturer))
{
    stateCatcher_.store(false, std::memory_order_release);
}

SignalCapturer::~SignalCapturer(){
    stop_reading();
}

void SignalCapturer::select_capturer(std::unique_ptr<APP::ICapturer> &&capturer){
    capturerPtr_ = std::move(capturer);
}

void SignalCapturer::start_reading(){
    if(!catcher_.joinable()){
        catcher_ = std::thread(&SignalCapturer::catch_loop, this);
        stateCatcher_.store(true, std::memory_order_release);
    }
}

void SignalCapturer::stop_reading(){
    stateCatcher_.store(false, std::memory_order_release);

    if(capturerPtr_)
        capturerPtr_->close_port();

    if(catcher_.joinable())
        catcher_.join();
}

void SignalCapturer::associate_screen(APP::IScreen* screenPtr){
    screenPtr_ = screenPtr;
    capturerPtr_->associate_screen(screenPtr);
}

void SignalCapturer::associate_voltages(DOMN::VoltageSignal* voltsPtr){
    voltages_ = voltsPtr;
}

void SignalCapturer::set_time_div(double timeDiv){
    if(capturerPtr_)
        capturerPtr_->set_data(&timeDiv);
}

std::mutex& SignalCapturer::get_mutex(){
    return mutexCatcher_;
}

void SignalCapturer::open_close_port(const char* portName){
    if(capturerPtr_){
        if(capturerPtr_->get_flag_serial()){
            capturerPtr_->close_port();
            if(screenPtr_)
                screenPtr_->update_port_state(false);
        }else{
            capturerPtr_->open_port(portName);
            if(screenPtr_)
                screenPtr_->update_port_state(true);
        }
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
        if(capturerPtr_)
            capturerPtr_->catch_data(this);

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
