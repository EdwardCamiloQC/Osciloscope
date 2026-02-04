#include <libraries/signal/signalObject.hpp>

//----------
//      PUBLIC METHODS
//----------
SignalObject::SignalObject(unsigned int len, SIGNAL_COLOR color)
    : abscissas(nullptr), vertex(nullptr), colorVertex(nullptr),
    vao(0), vbo{0,0}, length(len){
        abscissas = new float[length];
        vertex = new float[2*length];
        colorVertex = new float[3*length];
        organizeAbscissas();
        assignColor(color);
}

SignalObject::~SignalObject(){
    delete[] abscissas;
    delete[] vertex;
    delete[] colorVertex;
}

void SignalObject::updateVertex(const float* signal, float offset, float voltDiv){
    for(unsigned int i = 0; i < length; i++){
        vertex[2*i+1] = 0.25f*(signal[i]+offset)/voltDiv;
    }
}

//----------
//      PRIVATE METHODS
//----------
void SignalObject::organizeAbscissas(){
    abscissas[0] = -1.0f;
    vertex[0] = abscissas[0];
    for(unsigned int i = 1; i < length; i++){
        abscissas[i] = abscissas[i-1] + static_cast<float>(2.0f/(length-1.0f));
        vertex[2*i] = abscissas[i];
    }
}

void SignalObject::assignColor(SIGNAL_COLOR color){
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
    for(unsigned int i = 0; i < 3*length; i+=3){
        colorVertex[i] = r;
        colorVertex[i+1] = g;
        colorVertex[i+2] = b;
    }
}
