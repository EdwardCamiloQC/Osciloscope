#include <math.h>
#include <iostream>

#include <sys/peripherals/signalGenerator.hpp>
#include <sys/oscilloscope.hpp>

#define PI 3.14159265358979323846

SignalGenerator::SignalGenerator(){
}

SignalGenerator::~SignalGenerator(){
}

void SignalGenerator::readValues(VoltageSignal *volt1, [[maybe_unused]]VoltageSignal *volt2, [[maybe_unused]]VoltageSignal *volt3, [[maybe_unused]]VoltageSignal *volt4, unsigned int nValues){
    double deltaT = 8.0/(frequency*(128-1));
    double t = (deltaT/2) * (128-1);
    for(unsigned int i = volt1->length - nValues; i < volt1->length; i++){
        volt1->voltage[i] = static_cast<float>(sin(2.0*PI*frequency*t));
        t += deltaT;
    }
}

void SignalGenerator::setSampleFrequency(unsigned int freq){
    frequency = freq;
}

bool SignalGenerator::openPort([[maybe_unused]]const char* port){
    return 0;
}

bool SignalGenerator::closePort(){
    return 0;
}

bool SignalGenerator::getFlagSerial(){
    return 0;
}

TypeIdCapturer SignalGenerator::getId(){
    return GENERATE_SIGNAL_ID;
}
