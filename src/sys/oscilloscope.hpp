/** \file oscilloscope.hpp
    \brief The Oscilloscope was designed to display signals that arrive from the serial port.
    \author Edward Camilo
    \version v1.0
    \date enero 2026
 */
#ifndef _OSCILLOSCOPE_H_
    #define _OSCILLOSCOPE_H_

    #include <mutex>
    #include <sys/gui/screen.hpp>
    #include <sys/peripherals/signalCapturer.hpp>
    #include <sys/peripherals/puertoSerie.hpp>

    /** \brief Contain the principal components of the project.
            So there, are only one instance of this class.
        \class Oscilloscope
     */
    class Oscilloscope{
        // Attributes
        private:
            static Oscilloscope* instance_;
            static std::mutex mutex_;
        public:
            bool stateOnOff_; ///< State that indicates whether the application is running.
        private:
            Screen screen;
            SignalCapturer signalCapturer;
            ComSerial comSerial;
        
        // Methods
        private:
            /** \brief Creates the instance.
             */
            Oscilloscope();

            /** \brief Deletes the instance.
             */
            ~Oscilloscope();
        public:
            /** \note Copy-constructor disabled.
             */
            Oscilloscope(const Oscilloscope& oscilloscope) = delete;

            /** \note Assignment operator disabled.
             */
            Oscilloscope& operator=(Oscilloscope oscilloscope) = delete;

            /** \brief Gets the instance.
                \return Oscilloscope*: Returns the already created instance.
             */
            static Oscilloscope* getInstance();

            /** \brief Destroys the instance.
             */
            static void destroyInstance();

            /** \brief Runs the program.
                \param argc: Gets the argument count.
                \param argv: Gets the argument vector.
                \return Returns the any error with a int value.
             */
            int run(int& argc, char**& argv);
    };
#endif
