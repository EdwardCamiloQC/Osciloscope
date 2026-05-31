/** \file ISignalCapturer.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

#include <mutex>

namespace APP{
    struct IScreen;
}
namespace DOMN{
    class VoltageSignal;
}

/** \namespace APP.
 */
namespace APP{
    /** \interface ISignalCapturer.
     *  \struct
     */
    struct ISignalCapturer{
        virtual ~ISignalCapturer() = default;
        virtual void start_reading() = 0;
        virtual void stop_reading() = 0;
        virtual void associate_screen(APP::IScreen* screenPtr) = 0;
        virtual void associate_voltages(DOMN::VoltageSignal* voltsPtr) = 0;
        virtual void set_time_div(double timeDiv) = 0;
        virtual std::mutex& get_mutex() = 0;
        virtual void open_close_port(const char* portName) = 0;
    };
}
