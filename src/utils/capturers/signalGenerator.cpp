#include <math.h>
#include <iostream>

#include <utils/capturers/signalGenerator.hpp>
#include <oscilloscope.hpp>

#define PI 3.14159265358979323846

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalGenerator::SignalGenerator(){
}

SignalGenerator::~SignalGenerator(){
}

void SignalGenerator::read_values([[maybe_unused]]VoltageSignal *volt1, [[maybe_unused]]VoltageSignal *volt2, [[maybe_unused]]VoltageSignal *volt3, VoltageSignal *volt4, unsigned int nValues){
    unsigned int f = 1000;
    double deltaT = 8.0 / (f * volt4->length_);
    double t = 0.0;
    for(unsigned int i = volt1->length_ - nValues; i < volt1->length_; i++){
        volt4->pivotVoltage_pt_[i] = static_cast<float>(sin(2.0*PI*f*t));
        t += deltaT;
    }
}

void SignalGenerator::set_sample_frequency(unsigned int freq){
    frequency = freq;
}

int SignalGenerator::open_port([[maybe_unused]]const char* port){
    return 0;
}

int SignalGenerator::close_port(){
    return 0;
}

bool SignalGenerator::get_flag_serial(){
    return 0;
}

IdCapturer_t SignalGenerator::get_Id(){
    return GENERATE_SIGNAL_ID;
}
