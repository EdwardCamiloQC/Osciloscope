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
    while(osc->stateOnOff_){
        if(osc->stateStartStop_){
            if(capturer){
                //shift the voltages
                osc->voltage1.shiftVoltage(nValues);
                osc->voltage2.shiftVoltage(nValues);
                osc->voltage3.shiftVoltage(nValues);
                osc->voltage4.shiftVoltage(nValues);
                capturer->readValues(&(osc->voltage1),
                                    &(osc->voltage2),
                                    &(osc->voltage3),
                                    &(osc->voltage4),
                                    nValues);
                osc->voltage1.calculateSpectrum();
                osc->voltage2.calculateSpectrum();
                osc->voltage3.calculateSpectrum();
                osc->voltage4.calculateSpectrum();
            }else{
                std::cerr << "without concrete capturer" << std::endl;
            }
        }
        //std::this_thread::sleep_for(std::chrono::seconds(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(1000/(frequency*multiplier))));
    }
}
