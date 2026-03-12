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
            /** \brief Relates the capturer.
                \param theCapturer: Agregation with the capturer.
             */
            SignalCapturer(Capturer *theCapturer);

            /** \brief Destructor.
             */
            ~SignalCapturer();

            /** \brief Data capture begins.
                \param nValues: Amount of data points that the signal must shift.
             */
            void start(unsigned int nValues);

            /** \brief Sends the sample rate.
                \param valueFreq: value of the frequency without the multiplier.
             */
            void setTimeDiv(double tDiv);

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void selectCapturer(std::unique_ptr<Capturer> &&capturer);
        private:
            /** \brief Capture loop.
                \param nValues: Number of data points to capture.
             */
            void loopCatchVoltages(unsigned int nValues);

        //==========
        //~~~~~~~~~~
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        public:
            std::thread catcher; ///< Thread that catchs the data.
            std::unique_ptr<Capturer> capturer; ///< Instance to the especific capturer.
            double timeDiv {1.0};
    };
#endif
