/** \file capturer.hpp
    \author Edward Camilo
    \version v1.1
    \date April 2026
 */
#pragma once

class VoltageSignal;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TYPES DECLARATIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
/** \enum IdCapturer_t
    \class
 */
enum class IdCapturer_t{
    SERIAL_PORT_ID,
    SERIAL_PORT2_ID,
    GENERATE_SIGNAL_ID
};

/** \brief It is a interface that allows change between some catchers. 
    \struct
 */
struct Capturer{
    virtual ~Capturer() = default;

    virtual int open_port(const char *port) = 0;

    virtual int close_port() = 0;

    virtual void catch_data(void* userData) = 0;

    virtual void set_data(void* userData) = 0;

    virtual bool get_flag_serial() = 0;

    virtual IdCapturer_t get_Id() = 0;
};
