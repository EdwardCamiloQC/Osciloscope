/** \file spectrumSignal.hpp
 *  \author Edward Camilo
 *  \version v2.0
 *  \date June 2026
 */
#pragma once

#include <complex>
#include "domain/signals/signalObject.hpp"

/** \namespace DOMN.
 */
namespace DOMN{
    /** \brief Spectrum of a function.
        \class
     */
    class SpectrumSignal: public SignalObject{
        //====================
        // METHODS
        //====================
        public:
            /** \note Principal-Constructor disabled.
             */
            SpectrumSignal() = delete;

            /** \brief Constructor.
                \param len: Length of the spectrum signal.
                \param color: Signal color.
             */
            [[nodiscard]] explicit SpectrumSignal(unsigned int len, VAO_COLOR_t color);

            /** \note Copy-Constructor disabled.
             */
            SpectrumSignal(const SpectrumSignal& other) = delete;

            /** \note Move-Constructor disabled.
             */
            SpectrumSignal(SpectrumSignal&& other) = delete;

            /** \brief Destructor.
             */
            ~SpectrumSignal();

            /** \note Assignment operator disabled.
             */
            SpectrumSignal& operator=(const SpectrumSignal& other) = delete;
            SpectrumSignal& operator=(SpectrumSignal&& other) = delete;

            /** \brief Calcules the spectrum of a function.
                \param signal: Function to analyze.
             */
            void calculate_spectrum(const float *signal, unsigned int N);

            /** \brief Updates the vertives.
             */
            void update_vertex(unsigned int N);

        // Attributes
        private:
            float*                moduleSpectrumPtr_; ///< Module of a spectrum.
            std::complex<float>*  spectrumPtr_; ///< Spectrum.
    };
}
