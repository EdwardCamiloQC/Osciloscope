#include <utils/signal/spectrumSignal.hpp>
#include <utils/signal/fft.hpp>

SpectrumSignal::SpectrumSignal(unsigned int len, SIGNAL_COLOR color)
    :SignalObject(len, color), moduleSpectrum_pt_(nullptr), spectrum_pt_(nullptr){
        moduleSpectrum_pt_ = new float[length_];
        spectrum_pt_ = new std::complex<float>[length_];
}

SpectrumSignal::~SpectrumSignal(){
    delete[] moduleSpectrum_pt_;
    delete[] spectrum_pt_;
}

void SpectrumSignal::calculate_spectrum(const float *signal){
    SIGNALS::FFT::fft(signal, length_, spectrum_pt_);
    SIGNALS::FFT::calculateModule(spectrum_pt_, length_, moduleSpectrum_pt_);
}

void SpectrumSignal::update_vertex(){
    SignalObject::update_vertex(moduleSpectrum_pt_, -1.0f, 0.25f);
}
