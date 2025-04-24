#ifndef _OSCILLOSCOPE_H_
    #define _OSCILLOSCOPE_H_

    #include <mutex>
    #include <sys/gui/screen.hpp>
    #include <sys/peripherals/signalCapturer.hpp>
    #include <sys/peripherals/puertoSerie.hpp>

    class Oscilloscope{
        //      Methods     //
        private:
            Oscilloscope();
            ~Oscilloscope();
        public:
            Oscilloscope(const Oscilloscope& oscilloscope) = delete;
            Oscilloscope& operator=(Oscilloscope oscilloscope) = delete;
            static Oscilloscope* getInstance();
            static void destroyInstance();
            int run(int& argc, char**& argv);

        //      Attributes      //
        private:
            static Oscilloscope* instance_;
            static std::mutex mutex_;
        public:
            bool stateOnOff_;
        private:
            Screen screen;
            SignalCapturer signalCapturer;
            ComSerial comSerial;
    };
#endif
