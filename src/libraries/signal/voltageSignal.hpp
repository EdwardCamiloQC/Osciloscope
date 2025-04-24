#ifndef _VOLTAGE_SIGNAL_H_
    #define _VOLTAGE_SIGNAL_H_

    #include <libraries/signal/spectrumSignal.hpp>

    class VoltageSignal: public SignalObject{
        //  Methods
        public:
            VoltageSignal(unsigned int len, SIGNAL_COLOR color);
            ~VoltageSignal();
            VoltageSignal(const VoltageSignal&) = delete;
            VoltageSignal& operator=(VoltageSignal&) = delete;
            void applyOffset(const float &offset, const float &voldiv);
            void calculateSpectrum();
            float* getVoltage();
        private:
            void voltageToZero();
        //  Attributes
        private:
            float* voltage;
        public:
            SpectrumSignal spectrumSignal;
    };
#endif
