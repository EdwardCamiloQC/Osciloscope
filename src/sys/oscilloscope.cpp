#include <iostream>

#include <sys/oscilloscope.hpp>
#include <sys/peripherals/puertoSerie.hpp>

//----------
//      THREADS
//----------
gpointer inspectionTtyDevices(gpointer userData){
    std::cout << "hilo" << std::endl;
    return nullptr;
}

//----------
//----------
//      STATICS ATTRIBUTES
//----------
//----------
Oscilloscope* Oscilloscope::instance_ = nullptr;
std::mutex Oscilloscope::mutex_;

//----------
//----------
//      METHODS
//----------
//----------
Oscilloscope::Oscilloscope(): stateOnOff_(true),
    voltage1(128, SIGNAL_COLOR::RED),
    voltage2(128, SIGNAL_COLOR::ORANGE),
    voltage3(128, SIGNAL_COLOR::BLUE),
    voltage4(128, SIGNAL_COLOR::GREEN),
    screen(), signalCapturer(nullptr){
        voltage1.calculateSpectrum();
        voltage2.calculateSpectrum();
        voltage3.calculateSpectrum();
        voltage4.calculateSpectrum();
        signalCapturer.selectCapturer(std::make_unique<ComSerial>());
}

Oscilloscope::~Oscilloscope(){
}

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
    signalCapturer.start(static_cast<unsigned int>(voltage1.length/8));
    return screen.show(argc, argv);
}

