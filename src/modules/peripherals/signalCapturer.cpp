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
    :capturer(theCapturer){
}

SignalCapturer::~SignalCapturer(){
}

void SignalCapturer::set_timeDiv(double tDiv){
    capturer->setSampleFrequency(static_cast<unsigned int>(tDiv));
}

void SignalCapturer::select_capturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer = std::move(theCapturer);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void SignalCapturer::catch_voltages(VoltageSignal* volt1, VoltageSignal* volt2, VoltageSignal* volt3, VoltageSignal* volt4, unsigned int nValues){
    if(capturer){
        capturer->readValues(volt1, volt2, volt3, volt4, nValues);
    }else{
        std::cerr << "without concrete capturer" << std::endl;
    }
}
