/** \file IDevInspector.hpp
 *  \author Edward Camilo
 *  \version v1.0
 *  \date May 2026
 */
#pragma once

namespace APP{
    struct IScreen;
}

/** \namespace APP.
 */
namespace APP{
    /** \interface IDevInspector.
     *  \struct
     */
    struct IDevInspector{
        virtual ~IDevInspector() = default;
        virtual void associate_screen(APP::IScreen* screenPtr) = 0;
        virtual void init() = 0;
        virtual void stop() = 0;
    };
}
