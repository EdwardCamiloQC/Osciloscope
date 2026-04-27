/** \file spectrumSignal.hpp
    \author Edward Camilo
    \version v1.0
    \date January 2026
 */
#ifndef _SPECTRUM_SIGNAL_H_
    #define _SPECTRUM_SIGNAL_H_

    #include <entities/signals/signalObject.hpp>
    #include <complex>

    /** \brief Spectrum of a function.
        \class
     */
    class SpectrumSignal: public SignalObject{
        // Methods
        public:
            /** \brief Constructor.
                \param len: Length of the spectrum signal.
                \param color: Signal color.
             */
            SpectrumSignal(unsigned int len, SIGNAL_COLOR color);

            /** \note Copy constructor disabled.
             */
            SpectrumSignal(const SpectrumSignal&)=delete;

            /** \brief Destructor.
             */
            ~SpectrumSignal();

            /** \note Operator assignment disabled.
             */
            SpectrumSignal& operator=(SpectrumSignal&)=delete;

            /** \brief Calcules the spectrum of a function.
                \param signal: Function to analyze.
             */
            void calculate_spectrum(const float *signal);

            /** \brief Updates the vertives.
             */
            void update_vertex(void);

        // Attributes
        private:
            float *moduleSpectrum_pt_; ///< Module of a spectrum.
            std::complex<float> *spectrum_pt_; ///< Spectrum.
    };
#endif
