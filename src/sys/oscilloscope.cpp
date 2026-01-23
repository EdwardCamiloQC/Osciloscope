#include <sys/oscilloscope.hpp>
#include <iostream>
//#include <atomic>

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
Oscilloscope::Oscilloscope()
    : stateOnOff_(true),
    screen(),
    signalCapturer(&screen, nullptr){
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
    signalCapturer.start();
    return screen.show(argc, argv);
}

