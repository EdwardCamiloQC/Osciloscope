/** \file ICapturer.hpp
    \author Edward Camilo
    \version v1.1
    \date May 2026
 */
#pragma once

namespace APP{
    struct IScreen;
}

namespace APP{
    /** \enum MsgReturn_t.
     *  \class
     */
    enum class MsgReturn_t{
        PORT_OPENED,
        PORT_CLOSED,
        DONT_NAME_PORT,
        ERROR_IN_OPEN,
        ERROR_IN_CLOSE,
        ERROR_IN_CATCH,
        ERROR_IN_SEND,
        CATCH,
        SEND
    };

    /** \enum IdCapturer_t
        \class
    */
    enum class IdCapturer_t{
        SERIAL_PORT_PSOC_ID,
        SERIAL_PORT_ANY_ID,
        GENERATE_SIGNAL_ID
    };

    /** \brief It is a interface that allows change between some catchers. 
        \struct
    */
    struct ICapturer{
        virtual ~ICapturer() = default;
        virtual MsgReturn_t open_port(const char *port) = 0;
        virtual MsgReturn_t close_port() = 0;
        virtual MsgReturn_t catch_data(void* userData) = 0;
        virtual MsgReturn_t set_data(void* userData) = 0;
        virtual bool get_flag_serial() = 0;
        virtual IdCapturer_t get_Id() = 0;
    };
}
