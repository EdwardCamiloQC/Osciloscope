/** \file voltageSignal.hpp
    \author Edward Camilo
    \version v2.0
    \date March 2026
 */
#ifndef _VOLTAGE_SIGNAL_H_
    #define _VOLTAGE_SIGNAL_H_

    #include <utils/signal/spectrumSignal.hpp>

    /** \brief The class corresponding to the voltage.
        \class
     */
    class VoltageSignal: public SignalObject{
        //  Methods
        public:
            /** \note The basic constructor is disabled.
             */
             VoltageSignal() = delete;

            /** \brief Constructor.
                \param len: Length of the voltage signal data.
                \param color: Color of the voltage signal data.
             */
             [[nodiscard]] VoltageSignal(unsigned int len, SIGNAL_COLOR color);
            
            /** \note Copy-constructor disabled.
             */
             [[nodiscard]] VoltageSignal(const VoltageSignal &voltageSignal) = delete;
            
            /** \note Move-constructor disabled.
             */
             [[nodiscard]] VoltageSignal(VoltageSignal &&voltageSignal) = delete;

            /** \brief Destructor.
             */
             ~VoltageSignal();

            /** \note Operator assignment disabled.
             */
             [[nodiscard]] VoltageSignal& operator =(const VoltageSignal &voltageSignal) = delete;
             [[nodiscard]] VoltageSignal& operator =(VoltageSignal &&voltageSignal) = delete;

            /** \brief Applies the offset to the voltage.
                \param offset: Offset value.
                \param voldiv: volt/div.
             */
             void apply_offset(const float &offset, const float &voldiv);

            /** \brief Calculates the spectrum corresponding.
             */
             void calculate_spectrum();

            /** \brief Shifts the voltage n positions.
                \param n: Positions to shift.
             */
             void shift_voltage(unsigned int m);
            
            /** \brief Do pingpong.
             */
             void pivot_ping_pong_voltages();

        private:
            /** \brief Resets to zero to the signal.
             */
             void voltage_to_zero();

        private:
            float            *voltagePing_pt_;  ///< Voltage ping.
            float            *voltagePong_pt_;  ///< Voltage pong.
            bool             flagPingPong_;
        public:
            float            *pivotVoltage_pt_; ///< Pointer active of ping-pong.
            SpectrumSignal spectrumSignal_;     ///< Spectrum associated.
    };
#endif
