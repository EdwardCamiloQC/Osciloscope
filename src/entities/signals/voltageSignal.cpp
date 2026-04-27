#include <entities/signals/voltageSignal.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
VoltageSignal::VoltageSignal(unsigned int len, SIGNAL_COLOR color)
    : SignalObject(len, color),
    ringBuffer_(2*len),
    spectrumSignal_(len, color),
    bufferVoltagePt_(nullptr)
{
    bufferVoltagePt_ = new float[length_];
}

VoltageSignal::~VoltageSignal(){
    if(bufferVoltagePt_)
        delete[] bufferVoltagePt_;
}

void VoltageSignal::apply_offset(const float &offset, const float &voldiv, bool update){
    if(update)
        ringBuffer_.get_n_data(bufferVoltagePt_, length_);

    update_vertex(bufferVoltagePt_, offset, voldiv);
}

void VoltageSignal::calculate_spectrum(){
    spectrumSignal_.calculate_spectrum(bufferVoltagePt_);
}
