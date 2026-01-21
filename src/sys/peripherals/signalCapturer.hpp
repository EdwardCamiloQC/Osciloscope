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
    class Screen;

    /** \brief It collects the data it that receives.
        \class
     */
    class SignalCapturer{
        //  Methods
        public:
            /** \brief Relates the capturer and the screen.
                \param theScreen: Association with the screen.
                \param theCapturer: Agregation with the capturer.
             */
            SignalCapturer(Screen* theScreen, Capturer *theCapturer);

            /** \brief Destructor.
             */
            ~SignalCapturer();

            /** \brief Data capture begins.
             */
            void start();

            /** \brief Sends the sample rate.
             */
            void setSampleFrequency();
        private:
            /** \brief Capture loop.
             */
            void loop();

            /** \brief Catch data.
             */
            void catchVoltages();

            //void inspection();

        //  Attributes
        private:
            std::thread catcher; ///<
            Capturer *capturer; ///<
            Screen *screen; ///<
            //sd_device *devide;
    };
#endif
