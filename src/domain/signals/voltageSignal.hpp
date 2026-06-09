/** \file voltageSignal.hpp
    \author Edward Camilo
    \version v3.0
    \date May 2026
 */
#pragma once

#include "domain/signals/spectrumSignal.hpp"
#include "common/dataStructures/ringBuffer.hpp"

/** \namespace DOMN.
 */
namespace DOMN{
    /** \brief The class corresponding to the voltage.
        \class
    */
    class VoltageSignal: public SignalObject{
        //====================
        // METHODS
        //====================
        public:
            /** \note The basic constructor is disabled.
             */
            VoltageSignal() = delete;

            /** \brief Constructor.
                \param len: Length of the voltage signal data.
                \param color: Color of the voltage signal data.
            */
            [[nodiscard]] explicit VoltageSignal(unsigned int len, VAO_COLOR_t color);

            /** \note Copy-constructor disabled.
             */
            VoltageSignal(const VoltageSignal &other) = delete;
            
            /** \note Move-constructor disabled.
             */
            VoltageSignal(VoltageSignal &&other) = delete;

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
                \param M: Number of points that represents the signal in screen.
                \param update: This parameter indicates if use data updated or no.
            */
            void apply_changes(const float &offset, const float &voldiv, unsigned int M, bool update);

            /** \brief Calculates the spectrum corresponding.
             */
            void calculate_spectrum(unsigned int N);
        //====================
        // ATTRIBUTES
        //====================
        public:
            ED::RingBuffer ringBuffer_;  ///< Ring buffer of voltage.
            SpectrumSignal spectrumSignal_; ///< Spectrum associated.
        private:
            float*         bufferVoltagePtr_ {nullptr};
    };
}
