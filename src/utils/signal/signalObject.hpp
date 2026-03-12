#ifndef _SIGNAL_OBJECT_H_
    #define _SIGNAL_OBJECT_H_

    #include <epoxy/gl.h>

    enum class SIGNAL_COLOR{
        RED,
        GREEN,
        BLUE,
        YELLOW,
        ORANGE
    };

    class SignalObject{
        //  Methods
        public:
            SignalObject(unsigned int len, SIGNAL_COLOR color);
            SignalObject(const SignalObject&) = delete;
            SignalObject& operator=(SignalObject&) = delete;
            ~SignalObject();
            void updateVertex(const float* signal, float offset, float voltDiv);
        private:
            void organizeAbscissas();
            void assignColor(SIGNAL_COLOR);
        //  Attributes
        public:
            float *abscissas;
            float *vertex;
            float *colorVertex;
            GLuint vao, vbo[2];
            unsigned int length;
    };
#endif
