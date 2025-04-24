#ifndef _PUERTO_SERIE_H_
    #define _PUERTO_SERIE_H_

    //extern "C"{
        #include <SerialStream.h>
    //}

    #include <sys/peripherals/capturer.hpp>

    class VoltageSignal;

    class ComSerial: public Capturer{
        /*Methods*/
        public:
            ComSerial(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4);
            ComSerial(const ComSerial&) = delete;
            ComSerial& operator=(const ComSerial&) = delete;
            ~ComSerial();
            bool openPort(const char* port);
            bool closePort();
            bool getFlagSerial();
            void readValues(unsigned int nValues) override;
            void setSampleFrequency(unsigned int freq) override;
        private:
            void scrollVoltages(unsigned int n);
            bool startWith(std::string line, const char* text);

        /*Attributes*/
        private:
            LibSerial::SerialStream mySerial;
            VoltageSignal *voltage1 {nullptr};
            VoltageSignal *voltage2 {nullptr};
            VoltageSignal *voltage3 {nullptr};
            VoltageSignal *voltage4 {nullptr};
            float *valuesCatched1 {nullptr};
            float *valuesCatched2 {nullptr};
            float *valuesCatched3 {nullptr};
            float *valuesCatched4 {nullptr};
    };

#endif
