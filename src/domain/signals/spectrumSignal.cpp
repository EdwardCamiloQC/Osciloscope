#include "common/math/fft.hpp"
#include "domain/signals/spectrumSignal.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SpectrumSignal::SpectrumSignal(unsigned int len, VAO_COLOR_t color)
    :SignalObject(len, color), moduleSpectrumPtr_(nullptr), spectrumPtr_(nullptr)
{
        moduleSpectrumPtr_ = new float[length_];
        spectrumPtr_       = new std::complex<float>[length_];
}

SpectrumSignal::~SpectrumSignal(){
    if(moduleSpectrumPtr_)
        delete[] moduleSpectrumPtr_;

    if(spectrumPtr_)
        delete[] spectrumPtr_;
}

void SpectrumSignal::calculate_spectrum(const float *signal, unsigned int N){
    SIGNALS::FFT::fft(signal, N, spectrumPtr_);
    SIGNALS::FFT::calculateModule(spectrumPtr_, N, moduleSpectrumPtr_);
}

void SpectrumSignal::update_vertex(unsigned int N){
    if(N > get_numOfPoints())
        N = get_numOfPoints();

    SignalObject::update_vertices(moduleSpectrumPtr_, -1.0f, 0.25f, N);
}
