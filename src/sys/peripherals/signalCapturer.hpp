/** \file signalCapturer.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef _SIGNAL_CAPTURER_H_
    #define _SIGNAL_CAPTURER_H_

    #include <thread>
    /*extern "C"{
        #include <systemd/sd-device.h>
    }*/

    class Capturer;

    /** \brief It collects the data it that receives.
        \class
     */
    class SignalCapturer{
        //  Methods
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
            void setSampleFrequency(unsigned int freq);

            /** \brief Sets the multiplier correspondly to the frequency.
                \param mult: Multiplier frequency.
             */
            void setMultiplierFrequency(unsigned int mult);

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void selectCapturer(std::unique_ptr<Capturer> &&capturer);
        private:
            /** \brief Capture loop.
                \param nValues: Number of data points to capture.
             */
            void loopCatchVoltages(unsigned int nValues);

            //void inspection();

        //  Attributes
        public:
            std::thread catcher; ///< Thread that catchs the data.
            std::unique_ptr<Capturer> capturer; ///< Instance to the especific capturer.
            unsigned int frequency {50};
            unsigned int multiplier {1};
            //sd_device *devide;
    };
#endif
