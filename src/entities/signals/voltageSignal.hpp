/** \file voltageSignal.hpp
    \author Edward Camilo
    \version v3.0
    \date April 2026
 */
#pragma once

#include <entities/signals/spectrumSignal.hpp>
#include <common/dataStructures/ringBuffer.hpp>

/** \brief The class corresponding to the voltage.
    \class
 */
class VoltageSignal: public SignalObject{
    public:
        /** \note The basic constructor is disabled.
         */
        [[nodiscard]] VoltageSignal() = delete;

        /** \brief Constructor.
            \param len: Length of the voltage signal data.
            \param color: Color of the voltage signal data.
         */
        [[nodiscard]] VoltageSignal(unsigned int len, SIGNAL_COLOR color);
        
        /** \note Copy-constructor disabled.
         */
        [[nodiscard]] VoltageSignal(const VoltageSignal &other) = delete;
        
        /** \note Move-constructor disabled.
         */
        [[nodiscard]] VoltageSignal(VoltageSignal &&other) = delete;

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
            \param update: This parameter indicates if use data updated or no.
         */
        void apply_offset(const float &offset, const float &voldiv, bool update);

        /** \brief Calculates the spectrum corresponding.
         */
        void calculate_spectrum();

    public:
        ED::RingBuffer ringBuffer_;  ///< Ring buffer of voltage.
        SpectrumSignal spectrumSignal_; ///< Spectrum associated.

    private:
        float          *bufferVoltagePt_ {nullptr};
};
