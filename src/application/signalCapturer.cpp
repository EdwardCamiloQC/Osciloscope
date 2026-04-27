#include <application/signalCapturer.hpp>
#include <adapters/capturer.hpp>
#include <application/oscilloscope.hpp>

using namespace SIGNAL_CAPTURER;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalCapturer::SignalCapturer(std::unique_ptr<Capturer> theCapturer) : capturer_(std::move(theCapturer)){
}

SignalCapturer::~SignalCapturer(){
    if(capturer_)
        capturer_->close_port();

    if(catcher_.joinable())
        catcher_.join();
}

void SignalCapturer::select_capturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer_ = std::move(theCapturer);
}

void SignalCapturer::init(void *userData){
    if(!catcher_.joinable())
        catcher_ = std::thread(&SignalCapturer::catch_voltages, this, userData);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void SignalCapturer::catch_voltages([[maybe_unused]]void *userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    while(osc->stateOnOff_.load(std::memory_order_acquire)){
        capturer_->catch_data(osc);
    }
}
