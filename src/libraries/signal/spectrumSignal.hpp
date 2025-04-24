#ifndef _SPECTRUM_SIGNAL_H_
    #define _SPECTRUM_SIGNAL_H_

    #include <libraries/signal/signalObject.hpp>
    #include <complex>

    class SpectrumSignal: public SignalObject{
        public:
            SpectrumSignal(unsigned int len, SIGNAL_COLOR color);
            ~SpectrumSignal();
            SpectrumSignal(const SpectrumSignal&)=delete;
            SpectrumSignal& operator=(SpectrumSignal&)=delete;
            void calculateSpectrum(const float *signal);
        private:
            float *moduleSpectrum;
            std::complex<float> *spectrum;
    };
#endif
