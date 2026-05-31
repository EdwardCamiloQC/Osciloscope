/** \file signalObject.hpp
    \author Edward Camilo
    \version v1.0
    \date May 2026
 */
#pragma once

#include <epoxy/gl.h>

namespace DOMN{
    /** \enum
     *  \class SINNAL_COLOR
     */
    enum class SIGNAL_COLOR{
        RED,
        GREEN,
        BLUE,
        YELLOW,
        ORANGE
    };

    /** \class SignalObject.
     */
    class SignalObject{
        //====================
        // METHODS
        //====================
        public:
            SignalObject(unsigned int len, SIGNAL_COLOR color);
            SignalObject(const SignalObject&) = delete;
            SignalObject& operator=(SignalObject&) = delete;
            ~SignalObject();
            void update_vertex(const float* signal, float offset, float voltDiv);
            void assign_color(SIGNAL_COLOR);
        private:
            void organize_abscissas();
        //====================
        // ATTRIBUTES
        //====================
        public:
            float *abscissas_;
            float *vertex_;
            float *colorVertex_;
            GLuint vao_, vbo_[2];
            unsigned int length_;
    };
}
