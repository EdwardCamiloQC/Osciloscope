/** \file voltageSignal.hpp
    \author Edward Camilo
    \version v1.0
    \date January 2026
 */
#ifndef _VOLTAGE_SIGNAL_H_
    #define _VOLTAGE_SIGNAL_H_

    #include <libraries/signal/spectrumSignal.hpp>

    /** \brief The class corresponding to the voltage.
        \class
     */
    class VoltageSignal: public SignalObject{
        //  Methods
        public:
            /** \brief Constructor.
                \param len: Length of the voltage signal data.
                \param color: Color of the voltage signal data.
             */
            VoltageSignal(unsigned int len, SIGNAL_COLOR color);

            /** \brief Destructor.
             */
            ~VoltageSignal();

            /** \note Copy constructor disabled.
             */
            VoltageSignal(const VoltageSignal&) = delete;

            /** \note Operator assignment disabled.
             */
            VoltageSignal& operator=(VoltageSignal&) = delete;

            /** \brief Applies the offset to the voltage.
                \param offset: Offset value.
                \param voldiv: volt/div.
             */
            void applyOffset(const float &offset, const float &voldiv);

            /** \brief Calculates the spectrum corresponding.
             */
            void calculateSpectrum();

            /** \brief Shifts the voltage n positions.
                \param n: Positions to shift.
             */
            void shiftVoltage(unsigned int n);
        private:
            /** \brief Resets to zero to the signal.
             */
            void voltageToZero();

        //  Attributes
        public:
            float* voltage; ///< Voltage. The point to the signal.
            SpectrumSignal spectrumSignal; ///< Spectrum associated.
    };
#endif
