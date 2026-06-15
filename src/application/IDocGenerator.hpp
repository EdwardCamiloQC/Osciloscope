/** \file IDocGenerator.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

namespace DOMN{
    class VoltageSignal;
}

namespace APP{
    struct IScreen;
    struct ISignalCapturer;
}

/** \namespace APP.
 */
namespace APP{
    /** \enum TYPE_DOC
     *  \class
     */
    enum class TYPE_DOC{
        CSV,
        TSV,
        TXT,
        JSON
    };

    /** \interface IDocGenerator.
     *  \struct
     */
    struct IDocGenerator{
        virtual ~IDocGenerator() = default;
        virtual void associate_voltages(DOMN::VoltageSignal* voltsPtr) = 0;
        virtual void associate_screen(APP::IScreen* screenPtr) = 0;
        virtual void associate_signal_capturer(APP::ISignalCapturer* sigCapPtr) = 0;
        virtual void generate_doc(const char *directory, TYPE_DOC type, float timeDiv) const = 0;
    };
}
