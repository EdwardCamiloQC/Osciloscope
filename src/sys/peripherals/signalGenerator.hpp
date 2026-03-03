/** \file signalGenerator.hpp
    \author Edward Camilo
    \version v1.0
    \date January 2026
 */
#ifndef ED_SIGNAL_GENERATOR_HPP_
    #define ED_SIGNAL_GENERATOR_HPP_

    #include <sys/peripherals/capturer.hpp>

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // FORWARD DECLARATION
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    class VoltageSignal;

    //==================================================
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // CLASS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //==================================================
    /** \brief Generates test signals.
        \class
     */
    class SignalGenerator: public Capturer{
        //==========
        //~~~~~~~~~~
        // METHODS
        //~~~~~~~~~~
        //==========
        public:
            /** \brief Constructor.
             */
            SignalGenerator();

            /** \note Copy constructor disabled.
             */
            SignalGenerator(const SignalGenerator&) = delete;

            /** \note Operator assignment disabled.
             */
            SignalGenerator& operator=(const SignalGenerator&) = delete;

            /** \brief Destructor
             */
            ~SignalGenerator();

            /** \brief Generates the values and stored at the corresponding voltages.
                \param volt1: Reference to the voltage one.
                \param volt2: Reference to the voltage two.
                \param volt3: Reference to the voltage three.
                \param volt4: Reference to the voltage four.
                \param nValues: Data length to scrolling and storing.
                \override
             */
            void readValues(VoltageSignal *volt1, VoltageSignal *volt2,
                            VoltageSignal *volt3, VoltageSignal *volt4,
                            unsigned int nValues) override;

            /** \brief Sends the corresponding frequency.
                \param freq: Frequency.
                \override
             */
            void setSampleFrequency(unsigned int freq) override;

            /** \note This function does nothing.
             */
            int openPort(const char* port) override;

            /** \note This function does nothing.
             */
            int closePort() override;

            /** \note This function does nothing.
             */
            bool getFlagSerial() override;

            /** \brief Get id capturer.
                \return Id.
             */
            TypeIdCapturer getId() override;
        
        //==========
        //~~~~~~~~~~
        // ATTRIBUTES
        //~~~~~~~~~~
        //==========
        private:
            unsigned int frequency {1}; ///< Frequency with which the data is taken.
    };
#endif
