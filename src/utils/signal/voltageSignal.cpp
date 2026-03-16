#include <utils/signal/voltageSignal.hpp>

//----------
//      PUBLIC METHODS
//----------
VoltageSignal::VoltageSignal(unsigned int len, SIGNAL_COLOR color)
    : SignalObject(len, color), 
    voltagePing_pt_(nullptr), 
    voltagePong_pt_(nullptr),
    pivotVoltage_pt_(nullptr),
    spectrumSignal_(len, color)
{
    voltagePing_pt_ = new float[len];
    voltagePong_pt_ = new float[len];
    voltageToZero();
    flagPingPong_ = false;
    pivotVoltage_pt_ = voltagePing_pt_;
}

VoltageSignal::~VoltageSignal(){
    delete[] voltagePing_pt_;
    delete[] voltagePong_pt_;
    pivotVoltage_pt_ = nullptr;
}

void VoltageSignal::apply_offset(const float &offset, const float &voldiv){
    /*for(unsigned int i = 0; i < length; i++){
        voltage[i] += offset;
    }*/
    updateVertex(pivotVoltage_pt_, offset, voldiv);
}

void VoltageSignal::calculate_spectrum(){
    spectrumSignal_.calculateSpectrum(pivotVoltage_pt_);
}

void VoltageSignal::shift_voltage(unsigned int m){
    for(unsigned int i = 0; i < length-m; i++){
        pivotVoltage_pt_[i] = pivotVoltage_pt_[i+m];
    }
}

void VoltageSignal::pivot_ping_pong_voltages(){
    flagPingPong_ = !flagPingPong_;
    if(flagPingPong_){
        pivotVoltage_pt_ = voltagePong_pt_;
    }else{
        pivotVoltage_pt_ = voltagePing_pt_;
    }
}

//----------
//      PRIVATE METHODS
//----------
void VoltageSignal::voltageToZero(){
    for(unsigned int i = 0; i < length; i++){
        voltagePing_pt_[i] = 0.0f;
        voltagePong_pt_[i] = 0.0f;
    }
}
