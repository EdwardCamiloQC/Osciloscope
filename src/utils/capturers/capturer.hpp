/** \file capturer.hpp
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef ED_CAPTURER_H_
    #define ED_CAPTURER_H_

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // FORWARD DECLARATION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    class VoltageSignal;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TYPES DECLARATIONS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \typedef IdCapturer_t
        \enum
     */
     typedef enum{
        SERIAL_PORT_ID,
        GENERATE_SIGNAL_ID
     }IdCapturer_t;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief It is a interface that allows change between some catchers. 
        \class
     */
    class Capturer{
        public:
            virtual ~Capturer() = default;

            virtual void read_values(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4, unsigned int mValues) = 0;

            virtual void set_sample_frequency(unsigned int freq) = 0;

            virtual int open_port(const char *port) = 0;

            virtual int close_port() = 0;

            virtual bool get_flag_serial() = 0;

            virtual IdCapturer_t get_Id() = 0;
    };
#endif
