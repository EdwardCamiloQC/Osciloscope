#include <assert.h>
#include "domain/signals/voltageSignal.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
VoltageSignal::VoltageSignal(unsigned int len, VAO_COLOR_t color)
    : SignalObject(len, color),
    ringBuffer_(2*len),
    spectrumSignal_(len, color),
    bufferVoltagePtr_(nullptr)
{
    bufferVoltagePtr_ = new float[length_];
}

VoltageSignal::~VoltageSignal(){
    if(bufferVoltagePtr_)
        delete[] bufferVoltagePtr_;
}

void VoltageSignal::apply_changes(const float &offset, const float &voldiv, unsigned int M, bool update){
    assert(M >= 2);

    if(M > get_numOfPoints())
        M = get_numOfPoints();

    unsigned int n = get_numOfPoints();

    if(update){
        ringBuffer_.get_n_data(bufferVoltagePtr_, n);
    }

    update_vertices(bufferVoltagePtr_, offset, voldiv, M);
}

void VoltageSignal::calculate_spectrum(unsigned int N){
    spectrumSignal_.calculate_spectrum(bufferVoltagePtr_, N);
}
