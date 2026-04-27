#include <entities/signals/signalObject.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalObject::SignalObject(unsigned int len, SIGNAL_COLOR color)
    : abscissas_(nullptr), vertex_(nullptr), colorVertex_(nullptr),
    vao_(0), vbo_{0,0}, length_(len){
        abscissas_ = new float[length_];
        vertex_ = new float[2*length_];
        colorVertex_ = new float[3*length_];
        organize_abscissas();
        assign_color(color);
}

SignalObject::~SignalObject(){
    delete[] abscissas_;
    delete[] vertex_;
    delete[] colorVertex_;
}

void SignalObject::update_vertex(const float* signal, float offset, float voltDiv){
    for(unsigned int i = 0; i < length_; i++){
        vertex_[2*i+1] = 0.25f*(signal[i]+offset)/voltDiv;
    }
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================s
void SignalObject::organize_abscissas(){
    abscissas_[0] = -1.0f;
    vertex_[0] = abscissas_[0];
    for(unsigned int i = 1; i < length_; i++){
        abscissas_[i] = abscissas_[i-1] + static_cast<float>(2.0f/(length_-1.0f));
        vertex_[2*i] = abscissas_[i];
    }
}

void SignalObject::assign_color(SIGNAL_COLOR color){
    float r = 1.0f, g = 0.0f, b = 0.0f;
    switch(color){
        case SIGNAL_COLOR::RED:
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case SIGNAL_COLOR::GREEN:
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case SIGNAL_COLOR::BLUE:
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
            break;
        case SIGNAL_COLOR::YELLOW:
            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case SIGNAL_COLOR::ORANGE:
            r = 1.0f;
            g = 0.5f;
            b = 0.0f;
    }
    for(unsigned int i = 0; i < 3*length_; i+=3){
        colorVertex_[i] = r;
        colorVertex_[i+1] = g;
        colorVertex_[i+2] = b;
    }
}
