/** \file oscilloscope.hpp
    \brief The Oscilloscope was designed to display signals that arrive from the serial port.
    \author Edward Camilo
    \version v4.0
    \date April 2026
 */
#pragma once

#include <mutex>
#include <atomic>

#include <application/signalCapturer.hpp>
#include <application/inspectorDev.hpp>
#include <application/docGenerator.hpp>
#include <application/screen.hpp>
#include <entities/signals/voltageSignal.hpp>
#include <infrastructure/serialPort/serialPortPsoc.hpp>
#include <infrastructure/serialPort/serialPortMcu.hpp>
#include <infrastructure/gui/guiGTK.hpp>

namespace OSCILLOSCOPE{
    /** \brief The Oscilloscope displays four signals that arrive from the serial port.
        \class Oscilloscope.
     */
    class Oscilloscope{
        //====================
        // METHODS
        //====================
        public:
            /** \note Copy-constructor disabled.
             *  \param other: Oscilloscope object.
             */
            [[nodiscard]] Oscilloscope(const Oscilloscope& other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: Oscilloscope object.
             */
            [[nodiscard]] Oscilloscope(Oscilloscope &&other) = delete;

            /** \note Assignment operator disabled.
             *  \param other: Oscilloscope object.
             */
            [[nodiscard]] Oscilloscope& operator =(const Oscilloscope &other) = delete;
            [[nodiscard]] Oscilloscope& operator =(Oscilloscope &&other) = delete;

            /** \brief Operator bool.
             */
            [[nodiscard]] operator bool()const;

            /** \brief Gets the instance.
                \return Oscilloscope: Returns the already created instance.
             */
            static Oscilloscope* get_instance();

            /** \brief Destroys the instance.
             */
            static void destroy_instance();

            /** \brief Runs the program.
                \param argc: Argument count.
                \param argv: Argument vector.
                \return Returns the any error.
             */
            int run(int& argc, char**& argv);
        
        private:
            /** \note Constructor is private.
             *  \brief Creates the instance to the screen and a signal capturer.
             */
            [[nodiscard]] Oscilloscope();

            /** \note Destructor is private and by default.
             *  \brief Delete the instance.
             */
            ~Oscilloscope() = default;

        //====================
        // ATTRIBUTES
        //====================
        public:
            static constexpr unsigned long                LENGTH_VOLT_SIGNAL {1024};
            static constexpr unsigned int                 M_VALUES_TO_SHIFT  {1};
            inline static SIGNAL_CAPTURER::SignalCapturer signalCapturer_    {std::make_unique<SERIAL_PORT::SerialPortPsoc>()};
            inline static DEV_INSPECTOR::DevInspector     devInspector_      {};
            inline static DOC_GENERATOR::DocGenerator     docGenerator_      {};
            inline static SCREEN::Screen                  screen_            {std::make_unique<GUI_GTK::GuiGtk>(), 800, 800};
            inline static VoltageSignal                   voltage1_          {LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::RED};
            inline static VoltageSignal                   voltage2_          {LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::ORANGE};
            inline static VoltageSignal                   voltage3_          {LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::BLUE};
            inline static VoltageSignal                   voltage4_          {LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::GREEN};
            inline static std::mutex                      mutex_;
            inline static std::atomic<bool>               stateOnOff_        {false};
        private:
            inline static Oscilloscope                    *instancePt_       {nullptr};
     };
}
