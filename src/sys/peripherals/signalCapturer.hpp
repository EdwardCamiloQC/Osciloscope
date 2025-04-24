#ifndef _SIGNAL_CAPTURER_H_
    #define _SIGNAL_CAPTURER_H_

    #include <thread>
    /*extern "C"{
        #include <systemd/sd-device.h>
    }*/

    class Capturer;
    class Screen;

    class SignalCapturer{
        //  Methods
        public:
            SignalCapturer(Screen* theScreen, Capturer *theCapturer);
            ~SignalCapturer();
            void start();
            void setSampleFrequency();
        private:
            void loop();
            void catchVoltages();
            //void inspection();

        //  Attributes
        private:
            std::thread catcher;
            Capturer *capturer;
            Screen *screen;
            //sd_device *devide;
    };
#endif
