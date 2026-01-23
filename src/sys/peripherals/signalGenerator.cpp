#include <math.h>

#include <sys/peripherals/signalGenerator.hpp>

#define PI 3.14159265358979323846

SignalGenerator::SignalGenerator(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4):
    voltage1{volt1}, voltage2{volt2}, voltage3{volt3}, voltage4{volt4}{
}

SignalGenerator::~SignalGenerator(){
}

void SignalGenerator::readValues(unsigned int nValues){
    //shift the voltages
    voltage1->shiftVoltage(nValues);
    voltage2->shiftVoltage(nValues);
    voltage3->shiftVoltage(nValues);
    voltage4->shiftVoltage(nValues);
    //generating and assigning the voltages
    generateSamples(nValues);
}

void SignalGenerator::setSampleFrequency(unsigned int freq){
}

void SignalGenerator::generateSamples(unsigned int n){
    double x = 0.0;
    double delta = 2.0 * PI / n;
    for(unsigned int i = voltage1->length - n; i < voltage1->length; i++){
        voltage1->voltage[i] = static_cast<float>(sin(x));
        x += delta;
    }
}
