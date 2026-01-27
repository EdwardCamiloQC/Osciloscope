#include <math.h>

#include <sys/peripherals/signalGenerator.hpp>
#include <sys/oscilloscope.hpp>

#define PI 3.14159265358979323846

SignalGenerator::SignalGenerator(){
}

SignalGenerator::~SignalGenerator(){
}

void SignalGenerator::readValues(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4, unsigned int nValues){
    double x = 0.0;
    double delta = 2.0 * PI / nValues;
    for(unsigned int i = volt1->length - nValues; i < volt1->length; i++){
        volt1->voltage[i] = static_cast<float>(sin(x));
        x += delta;
    }
}

void SignalGenerator::setSampleFrequency(unsigned int freq){
}
