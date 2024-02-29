#ifndef _PUERTO_SERIE_H_
    #define _PUERTO_SERIE_H_

    #include <SerialStream.h>
    #include <string>

    bool startWith(std::string line, const char* text);

    class ComSerial{
        /*Methods*/
        public:
            ComSerial(const char* port);
            void run(unsigned short scroll);
            ~ComSerial();
        private:
            void initSignalsToZero();
            void assignColorToSignals();
            void openPort(const char* port);
            void scrollSignals(unsigned short landslide);
        public:
            float dataVoltage1[800];
            float colorSignal1[1200];
            float dataVoltage2[800];
            float colorSignal2[1200];
        private:
            LibSerial::SerialStream mySerial;
            unsigned int signal1, signal2;
            float voltage;
            bool flagOpen;
    };

#endif
