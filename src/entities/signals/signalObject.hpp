/** \file signalObject.hpp
    \author Edward Camilo
    \version v1.0
    \date March 2026
 */
#ifndef _SIGNAL_OBJECT_H_
    #define _SIGNAL_OBJECT_H_

    #include <epoxy/gl.h>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TYPES DECLARATIONS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
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

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief
        \class
     */
    class SignalObject{
        //========== METHODS ==========
        public:
            SignalObject(unsigned int len, SIGNAL_COLOR color);
            SignalObject(const SignalObject&) = delete;
            SignalObject& operator=(SignalObject&) = delete;
            ~SignalObject();
            void update_vertex(const float* signal, float offset, float voltDiv);
        private:
            void organize_abscissas();
            void assign_color(SIGNAL_COLOR);
        //========== ATTRIBUTES ==========
        public:
            float *abscissas_;
            float *vertex_;
            float *colorVertex_;
            GLuint vao_, vbo_[2];
            unsigned int length_;
    };
#endif
