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
             */
            void start();

            /** \brief Sends the sample rate.
             */
            void setSampleFrequency();

            /** \brief Selects between the capturers.
                \param theCapturer: The capturer to use.
             */
            void selectCapturer(std::unique_ptr<Capturer> &&capturer);
        private:
            /** \brief Capture loop.
             */
            void loopCatchVoltages();

            //void inspection();

        //  Attributes
        private:
            std::thread catcher; ///< Thread that catchs the data.
            std::unique_ptr<Capturer> capturer; ///< Instance to the especific capturer.
            //sd_device *devide;
    };
#endif
