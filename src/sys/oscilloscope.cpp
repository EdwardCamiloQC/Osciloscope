#include <iostream>

#include <sys/oscilloscope.hpp>
#include <sys/peripherals/puertoSerie.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// STATICS ATTRIBUTES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope* Oscilloscope::instance_ = nullptr;
std::mutex Oscilloscope::mutex_;
std::atomic<bool> Oscilloscope::stateOnOff_ {false};
std::atomic<bool> Oscilloscope::stateStartStop_ {false};

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::Oscilloscope(unsigned int lenSignals = 128):
    voltage1_(lenSignals, SIGNAL_COLOR::RED),
    voltage2_(lenSignals, SIGNAL_COLOR::ORANGE),
    voltage3_(lenSignals, SIGNAL_COLOR::BLUE),
    voltage4_(lenSignals, SIGNAL_COLOR::GREEN),
    screen_(), signalCapturer_(nullptr)
{
    stateOnOff_.store(true, std::memory_order_release);
    voltage1_.calculateSpectrum();
    voltage2_.calculateSpectrum();
    voltage3_.calculateSpectrum();
    voltage4_.calculateSpectrum();
    signalCapturer_.selectCapturer(std::make_unique<ComSerial>());
    signalCapturer_.setSampleFrequency(50);
    signalCapturer_.setMultiplierFrequency(1);
}

Oscilloscope::~Oscilloscope(){
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope* Oscilloscope::getInstance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instance_ == nullptr){
        instance_ = new Oscilloscope();
        std::atexit(destroyInstance);
    }
    return instance_;
}

void Oscilloscope::destroyInstance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instance_ != nullptr){
        delete instance_;
        instance_ = nullptr;
    }
}

int Oscilloscope::run(int &argc, char**& argv){
    signalCapturer_.start(static_cast<unsigned int>(voltage1_.length/32));
    return screen_.show(argc, argv);
}
