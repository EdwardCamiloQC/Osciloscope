#include <libraries/signal/voltageSignal.hpp>

//----------
//      PUBLIC METHODS
//----------
VoltageSignal::VoltageSignal(unsigned int len, SIGNAL_COLOR color)
    : SignalObject(len, color), voltage(nullptr), spectrumSignal(len, color){
    voltage = new float[len];
}

VoltageSignal::~VoltageSignal(){
    delete[] voltage;
}

void VoltageSignal::applyOffset(const float &offset, const float &voldiv){
    /*for(unsigned int i = 0; i < length; i++){
        voltage[i] += offset;
    }*/
    updateVertex(voltage, offset, voldiv);
}

void VoltageSignal::calculateSpectrum(){
    spectrumSignal.calculateSpectrum(voltage);
}

float* VoltageSignal::getVoltage(){
    return voltage;
}

//----------
//      PRIVATE METHODS
//----------
void VoltageSignal::voltageToZero(){
    for(unsigned int i = 0; i < length; i++){
        voltage[i] = 0.0f;
    }
}
