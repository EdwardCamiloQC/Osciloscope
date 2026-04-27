#include <chrono>

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
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();
    /*using clock = std::chrono::steady_clock;
    using msD = std::chrono::duration<double, std::milli>;

    double timeDiv = timeDiv_.load(std::memory_order_acquire);
    const double timeWindow = static_cast<double>(8) * static_cast<double>(timeDiv);
    const double timeCapture = (4.0 * static_cast<double>(M_VALUES_TO_SHIFT) * timeWindow) / (static_cast<double>(LENGTH_VOLT_SIGNAL));

    const clock::duration period = std::chrono::duration_cast<clock::duration>(msD{timeCapture});

    auto nextDeadLine = clock::now();*/

    while(osc->stateOnOff_.load(std::memory_order_acquire)){
        capturer_->catch_data(osc);

        /*nextDeadLine += period;
        const auto now = clock::now();
        if(now < nextDeadLine){
            std::this_thread::sleep_until(nextDeadLine);
        }else{
            do{
                nextDeadLine += period;
            }while(nextDeadLine <= now);
        }*/
    }
}
