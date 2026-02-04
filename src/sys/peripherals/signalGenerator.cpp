#include <math.h>
#include <iostream>

#include <sys/peripherals/signalGenerator.hpp>
#include <sys/oscilloscope.hpp>

#define PI 3.14159265358979323846

SignalGenerator::SignalGenerator(){
}

SignalGenerator::~SignalGenerator(){
}

void SignalGenerator::readValues(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4, unsigned int nValues){
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
