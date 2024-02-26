#ifndef _PUERTO_SERIE_H_
    #define _PUERTO_SERIE_H_

    #include <SerialStream.h>
    #include <string>

    bool startWith(std::string line, const char* text);

    class ComSerial{
        public:
            ComSerial(const char* port);
            void run();
            void scrollSignal();
            ~ComSerial();
        public:
            float dataVoltage[400];
        private:
            LibSerial::SerialStream mySerial;
            unsigned int valueADC;
            float voltage;
            bool flagOpen;
    };

#endif
