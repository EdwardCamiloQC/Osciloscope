/** \file IGui.hpp
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
    /** \interface IGui.
     *  \struct Gui (Graphic User Interface)
     */
    struct IGui{
        virtual ~IGui() = default;
        virtual int init(int &argc, char **&argv) = 0;
        virtual void associate_signal_capturer(ISignalCapturer* sigCapPtr) = 0;
        virtual void associate_dev_inspector(IDevInspector* devInspPtr) = 0;
        virtual void associate_doc_generator(IDocGenerator* docGenPtr) = 0;
        virtual void associate_voltages(DOMN::VoltageSignal* voltsPtr) = 0;
        virtual void update_drop_port(bool add, const char *portDevice) = 0;
        virtual void update_label_button_port(bool state) = 0;
        virtual void display_message(const char* msg, int type)const = 0;
    };
}
