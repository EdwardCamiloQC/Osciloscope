/** \file capturer.hpp
    \brief It collects the data it that receives.
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef _CAPTURER_H_
    #define _CAPTURER_H_

    /** \brief It collects the data it that receives. 
        \class
     */
    class Capturer{
        public:
            /** \note This is a virtual method (destructor).
             */
            virtual ~Capturer() = default;

            /** \brief Reads the values that it arrives.
                \param nValues: Number of data to read.
             */
            virtual void readValues(unsigned int nValues) = 0;

            /** \brief Sets the sampling rate to acquire the data for the external hardware.
                \param freq: Frequency.
             */
            virtual void setSampleFrequency(unsigned int freq) = 0;
    };
#endif
