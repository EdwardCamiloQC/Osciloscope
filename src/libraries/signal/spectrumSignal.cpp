#include <libraries/signal/spectrumSignal.hpp>
#include <libraries/signal/fft.hpp>

SpectrumSignal::SpectrumSignal(unsigned int len, SIGNAL_COLOR color)
    :SignalObject(len, color), moduleSpectrum(nullptr), spectrum(nullptr){
        moduleSpectrum = new float[length];
        spectrum = new std::complex<float>[length];
}

SpectrumSignal::~SpectrumSignal(){
    delete[] moduleSpectrum;
    delete[] spectrum;
}

void SpectrumSignal::calculateSpectrum(const float *signal){
    fft(signal, length, spectrum, true);
    calculateModule(spectrum, length, moduleSpectrum);
}

void SpectrumSignal::updateVertex(){
    SignalObject::updateVertex(moduleSpectrum, -1.0f, 0.25f);
}
