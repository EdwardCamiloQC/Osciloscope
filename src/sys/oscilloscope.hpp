/** \file oscilloscope.hpp
    \brief The Oscilloscope was designed to display signals that arrive from the serial port.
    \author Edward Camilo
    \version v3.0
    \date February 2026
 */
#ifndef ED_OSCILLOSCOPE_H_
    #define ED_OSCILLOSCOPE_H_

    #include <mutex>
    #include <atomic>

    #include <libraries/signal/voltageSignal.hpp>
    #include <sys/gui/screen.hpp>
    #include <sys/peripherals/signalCapturer.hpp>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief The Oscilloscope class displays four signals that arrive from the serial port.
            So there, are only one instance of this class.
        \class Oscilloscope.
     */
    class Oscilloscope{
        //==========
        //~~~~~~~~~~
        // METHODS
        //~~~~~~~~~~
        //==========
        private:
            /** \brief Creates the instance.
             */
            Oscilloscope(unsigned int lenSignals);

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
        
        //==========
        //~~~~~~~~~~ 
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        private:
            static Oscilloscope        *instance_; ///< Unique instance of the project.
        public:
            static std::mutex          mutex_;
            static std::atomic<bool>   stateOnOff_; ///< State that indicates whether the application is running.
            static std::atomic<bool>   stateStartStop_; ///< State that indicates when to update the signals or not.
            VoltageSignal              voltage1_; ///< Voltage one.
            VoltageSignal              voltage2_; ///< Voltage two.
            VoltageSignal              voltage3_; ///< Voltage three.
            VoltageSignal              voltage4_; ///< Voltage four.
            Screen                     screen_; ///< Screen where shows the signals and configures some parameters.
            SignalCapturer             signalCapturer_; ///< This is the capturer that stores the data signals.
    };
#endif
