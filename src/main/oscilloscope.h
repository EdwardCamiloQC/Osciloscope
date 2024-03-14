#ifndef _PRINCIPAL_WINDOW_H_
    #define _PRINCIPAL_WINDOW_H_

    #include <gtk/gtk.h>
    #include <shaders.h>
    #include <puertoSerie.h>

    class Oscilloscope{
        //      Methods     //
        public:
            Oscilloscope(int& argc, char**& argv, const char* port);
            int oscilloscope_run(int& argc, char**& argv);
            ~Oscilloscope();

        //      Attributes      //
        public:
            ComSerial psoc;
        private:
            GtkApplication *oscilloscope;
    };
#endif
