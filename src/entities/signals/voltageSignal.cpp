#include <entities/signals/voltageSignal.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
VoltageSignal::VoltageSignal(unsigned int len, SIGNAL_COLOR color)
    : SignalObject(len, color),
    ringBuffer_(2*len),
    spectrumSignal_(len, color)
{
    //
}

VoltageSignal::~VoltageSignal(){
    //
}

void VoltageSignal::apply_offset(const float &offset, const float &voldiv){
    update_vertex(ringBuffer_.get_n_data(length_), offset, voldiv);
}

void VoltageSignal::calculate_spectrum(){
    spectrumSignal_.calculate_spectrum(ringBuffer_.get_n_data(length_));
}
