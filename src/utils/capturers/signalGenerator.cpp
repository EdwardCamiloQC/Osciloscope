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

void SignalGenerator::readValues([[maybe_unused]]VoltageSignal *volt1, [[maybe_unused]]VoltageSignal *volt2, [[maybe_unused]]VoltageSignal *volt3, VoltageSignal *volt4, unsigned int nValues){
    unsigned int f = 1000;
    double deltaT = 8.0 / (f * volt4->length);
    double t = 0.0;
    for(unsigned int i = volt1->length - nValues; i < volt1->length; i++){
        volt4->pivotVoltage_pt_[i] = static_cast<float>(sin(2.0*PI*f*t));
        t += deltaT;
    }
}

void SignalGenerator::setSampleFrequency(unsigned int freq){
    frequency = freq;
}

int SignalGenerator::openPort([[maybe_unused]]const char* port){
    return 0;
}

int SignalGenerator::closePort(){
    return 0;
}

bool SignalGenerator::getFlagSerial(){
    return 0;
}

TypeIdCapturer SignalGenerator::getId(){
    return GENERATE_SIGNAL_ID;
}
