#include <iostream>
#include <chrono>

#include <sys/peripherals/signalCapturer.hpp>
#include <sys/oscilloscope.hpp>
#include <sys/peripherals/capturer.hpp>

//~~~~~~~~~~
//      PUBLIC METHODS
//~~~~~~~~~~
SignalCapturer::SignalCapturer(Capturer *theCapturer)
    :capturer(theCapturer){
}

SignalCapturer::~SignalCapturer(){
    if(catcher.joinable()){
        catcher.join();
    }
}

void SignalCapturer::start(unsigned int nValues){
    catcher = std::thread(&SignalCapturer::loopCatchVoltages, this, nValues);
}

void SignalCapturer::setSampleFrequency(unsigned int freq){
    frequency = freq;
    capturer->setSampleFrequency(frequency * multiplier);
}

void SignalCapturer::setMultiplierFrequency(unsigned int mult){
    multiplier = mult;
    capturer->setSampleFrequency(frequency * multiplier);
}

void SignalCapturer::selectCapturer(std::unique_ptr<Capturer> &&theCapturer){
    capturer = std::move(theCapturer);
}

//~~~~~~~~~~
//      PRIVATE METHODS
//~~~~~~~~~~
void SignalCapturer::loopCatchVoltages(unsigned int nValues){
    Oscilloscope *osc = Oscilloscope::getInstance();
    while(osc->stateOnOff_.load(std::memory_order_acquire)){
        if(osc->stateStartStop_.load(std::memory_order_acquire)){
            {
                std::lock_guard<std::mutex> lock(osc->mutex_);
                if(capturer){
                    //shift the voltages
                    osc->voltage1_.shiftVoltage(nValues);
                    osc->voltage2_.shiftVoltage(nValues);
                    osc->voltage3_.shiftVoltage(nValues);
                    osc->voltage4_.shiftVoltage(nValues);
                    capturer->readValues(&(osc->voltage1_),
                                        &(osc->voltage2_),
                                        &(osc->voltage3_),
                                        &(osc->voltage4_),
                                        nValues);
                    osc->voltage1_.calculateSpectrum();
                    osc->voltage2_.calculateSpectrum();
                    osc->voltage3_.calculateSpectrum();
                    osc->voltage4_.calculateSpectrum();
                }else{
                std::cerr << "without concrete capturer" << std::endl;
                }
            }
        }
        //std::this_thread::sleep_for(std::chrono::seconds(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(1000/(frequency*multiplier))));
    }
}
