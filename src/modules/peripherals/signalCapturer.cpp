#include <iostream>
#include <chrono>

#include <modules/peripherals/signalCapturer.hpp>
#include <oscilloscope.hpp>
#include <utils/capturers/capturer.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalCapturer::SignalCapturer(){
}

SignalCapturer::SignalCapturer(Capturer *theCapturer = nullptr)
    :capturer_(theCapturer){
}

SignalCapturer::~SignalCapturer(){
}

void SignalCapturer::select_capturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer_ = std::move(theCapturer);
}
