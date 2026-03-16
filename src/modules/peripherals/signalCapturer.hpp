/** \file signalCapturer.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef ED_SIGNAL_CAPTURER_H_
    #define ED_SIGNAL_CAPTURER_H_

    #include <thread>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // FORWARD DECLARATION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    class Capturer;
    class VoltageSignal;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief It collects the data it that receives.
        \class
     */
    class SignalCapturer{
        //==========
        //~~~~~~~~~~
        // METHODS
        //~~~~~~~~~~
        //==========
        public:
            /**
             */
            SignalCapturer();
            
            /** \brief Relates the capturer.
                \param theCapturer: Agregation with the capturer.
             */
            SignalCapturer(Capturer *theCapturer);

            /** \brief Destructor.
             */
            ~SignalCapturer();

            /** \brief Sends the sample rate.
                \param valueFreq: value of the frequency without the multiplier.
             */
            void set_timeDiv(double tDiv);

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void select_capturer(std::unique_ptr<Capturer> &&capturer);
            
            /** \brief Capture loop.
                \param nValues: Number of data points to capture.
             */
            void catch_voltages(VoltageSignal* volt1, VoltageSignal* volt2, VoltageSignal* volt3, VoltageSignal* volt4, unsigned int nValues);

        //==========
        //~~~~~~~~~~
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        public:
            std::unique_ptr<Capturer> capturer; ///< Instance to the especific capturer.
    };
#endif
