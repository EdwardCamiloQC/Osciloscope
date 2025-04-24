#ifndef _CAPTURER_H_
    #define _CAPTURER_H_

    class Capturer{
        public:
            virtual ~Capturer() = default;
            virtual void readValues(unsigned int nValues) = 0;
            virtual void setSampleFrequency(unsigned int freq) = 0;
    };
#endif
