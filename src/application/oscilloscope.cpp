#include <glib-unix.h>
#include <memory>

#include <application/oscilloscope.hpp>

using namespace OSCILLOSCOPE;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::operator bool()const{
    return instancePt_ != nullptr;
}

Oscilloscope* Oscilloscope::get_instance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instancePt_ == nullptr){
        instancePt_ = new Oscilloscope();
        std::atexit(destroy_instance);
    }
    return instancePt_;
}

void Oscilloscope::destroy_instance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instancePt_ != nullptr){
        delete instancePt_;
        instancePt_ = nullptr;
    }
}

int Oscilloscope::run(int& argc, char**& argv){
    return screen_.turn_on(argc, argv);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::Oscilloscope(){
    //
}
