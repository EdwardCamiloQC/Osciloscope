/** \file IScreen.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

namespace APP{
    struct ISignalCapturer;
    struct IDevInspector;
    struct IDocGenerator;
}
namespace DOMN{
    class VoltageSignal;
}

/** \namespace APP.
 */
namespace APP{
    /** \interface IScreen.
     *  \struct
     */
    struct IScreen{
        virtual ~IScreen() = default;
        virtual void associate_signal_capturer(APP::ISignalCapturer* signalCapturerPtr) = 0;
        virtual void associate_dev_inspector(APP::IDevInspector* devInspPtr) = 0;
        virtual void associate_doc_generator(APP::IDocGenerator* docGenPtr) = 0;
        virtual void associate_voltages(DOMN::VoltageSignal* voltsPtr) = 0;
        virtual int turn_on(int &argc, char **&argv) = 0;
        virtual int turn_off() = 0;
        virtual void add_device(const char* devName) = 0;
        virtual void remove_device(const char* devName) = 0;
        virtual void update_port_state(bool state) = 0;
        virtual void set_message(const char* msg, int type) const = 0;
        virtual void set_displacement_time(long sec, long nsec) = 0;
    };
}
