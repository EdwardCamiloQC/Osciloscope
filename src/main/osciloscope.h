#ifndef _PRINCIPALWINDOW_H_
    #define _PRINCIPALWINDOW_H_

    #include <GLFW/glfw3.h>
    #include <stdint.h>
    #include <puertoSerie.h>
    #include <grid.h>

    #define VIEW_2D_ENABLE

    class WindowOsciloscope{
        public:
            WindowOsciloscope(uint16_t width, uint16_t height, const char* port);
            WindowOsciloscope(const WindowOsciloscope& w);
            bool getFlagRun();
            ~WindowOsciloscope();
            void run();
        public:
            uint32_t ancho;
            uint32_t alto;
            const char* NAME_WINDOW = "Osciloscopio";
            ComSerial psoc;
        private:
            GLFWwindow* window;
            GLuint VAOs[2], VBOs[2];
            float backgroundColor[4];
            unsigned int idP;
            struct Cuadricula grid;
            bool flagRun;
    };

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void inputKeys(GLFWwindow* window);
#endif