#ifndef _PUERTO_SERIE_H_
    #define _PUERTO_SERIE_H_

    #include <SerialStream.h>
    #include <string>

    bool startWith(std::string line, const char* text);

    class ComSerial{
        /*Methods*/
        public:
            ComSerial(const char* port);
            void run();
            ~ComSerial();
        private:
            void scrollSignal(unsigned short landslide);
        public:
            float dataVoltage1[600];
        private:
            LibSerial::SerialStream mySerial;
            unsigned int signal1, signal2;
            float voltage;
            bool flagOpen;
    };

#endif
