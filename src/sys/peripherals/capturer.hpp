/** \file capturer.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef _CAPTURER_H_
    #define _CAPTURER_H_

    //Forward declaration
    class VoltageSignal;

    typedef enum{
        SERIAL_PORT_ID,
        GENERATE_SIGNAL_ID
    }TypeIdCapturer;

    /** \brief It is a interface that allows change between some catchers. 
        \class
     */
    class Capturer{
        public:
            /** \note This is a virtual method (destructor).
             */
            virtual ~Capturer() = default;

            /** \brief Reads the values that it arrives.
                \param volt1: Reference to the voltage one.
                \param volt2: Reference to the voltage two.
                \param volt3: Reference to the voltage three.
                \param volt4: Reference to the voltage four.
                \param nValues: Number of data to read.
             */
            virtual void readValues(VoltageSignal *volt1, VoltageSignal *volt2,
                                    VoltageSignal *volt3, VoltageSignal *volt4,
                                    unsigned int nValues) = 0;

            /** \brief Sets the sampling rate to acquire the data for the external hardware.
                \param freq: Frequency.
             */
            virtual void setSampleFrequency(unsigned int freq) = 0;

            virtual bool openPort(const char *port) = 0;

            virtual bool closePort() = 0;

            virtual bool getFlagSerial() = 0;

            virtual TypeIdCapturer getId() = 0;
    };
#endif
