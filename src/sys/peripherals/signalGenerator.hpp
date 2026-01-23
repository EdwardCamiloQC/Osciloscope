/** \file signalGenerator.hpp
    \author Edward Camilo
    \version v1.0
    \date January 2026
 */
#ifndef _SIGNAL_GENERATOR_HPP_
    #define _SIGNAL_GENERATOR_HPP_

    #include <sys/peripherals/capturer.hpp>
    #include <libraries/signal/voltageSignal.hpp>

    /** \brief Generates test signals.
        \class
     */
    class SignalGenerator: public Capturer{
        //Attributes
        private:
            VoltageSignal *voltage1 {nullptr}; ///< Association with voltage1.
            VoltageSignal *voltage2 {nullptr}; ///< Association with voltage2.
            VoltageSignal *voltage3 {nullptr}; ///< Association with voltage3.
            VoltageSignal *voltage4 {nullptr}; ///< Association with voltage4.
            unsigned int frequency {1}; ///< Frequency with which the data is taken.

        //Methods
        public:
            /** \brief Constructor.
             */
            SignalGenerator(VoltageSignal *volt1, VoltageSignal *volt2, VoltageSignal *volt3, VoltageSignal *volt4);

            /** \note Copy constructor disabled.
             */
            SignalGenerator(const SignalGenerator&) = delete;

            /** \note Operator assignment disabled.
             */
            SignalGenerator& operator=(const SignalGenerator&) = delete;

            /** \brief Destructor
             */
            ~SignalGenerator();

            /** \brief Reads the values and stored at the corresponding voltages.
                \param nValues: Data length to scrolling and storing.
                \override
             */
            void readValues(unsigned int nValues) override;

            /** \brief Sends the corresponding frequency.
                \param freq: Frequency.
                \override
             */
            void setSampleFrequency(unsigned int freq) override;
        private:
            /** \brief Generates the signals.
                \param n: Length of the data to generating.
             */
            void generateSamples(unsigned int n);
    };
#endif
