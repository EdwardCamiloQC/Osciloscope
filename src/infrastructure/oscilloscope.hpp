/** \file oscilloscope.hpp
 *  \brief The Oscilloscope was designed to display signals that arrive from the serial port.
 *  \author Edward Camilo
 *  \version v4.0
 *  \date May 2026
 */
#pragma once

#include <memory>
#include "application/ISignalCapturer.hpp"
#include "application/IDevInspector.hpp"
#include "application/IDocGenerator.hpp"
#include "application/IScreen.hpp"
#include "domain/signals/voltageSignal.hpp"

namespace DOM_SIGNAL{
    class VoltageSignal;
}

/** \namespace INFRA.
 */
namespace INFRA{
    /** \class Oscilloscope.
     */
    class Oscilloscope{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             *  Defines the SignalCapturer, DevInspector, DocGenerator, Screen and Voltages.
             *  Also its associations.
             */
            [[nodiscard]] Oscilloscope();

            /** \note Destructor by default.
             */
            ~Oscilloscope() = default;

            /** \note Copy-constructor disabled.
             *  \param other: Oscilloscope object.
             */
            Oscilloscope(const Oscilloscope& other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: Oscilloscope object.
             */
            Oscilloscope(Oscilloscope &&other) = delete;

            /** \note Assignment operator disabled.
             *  \param other: Oscilloscope object.
             *  \return Oscilloscope object.
             */
            Oscilloscope& operator =(const Oscilloscope &other) = delete;
            Oscilloscope& operator =(Oscilloscope &&other) = delete;

            /** \brief Runs the program.
             *  Initializes the Screen that it initializes the rest.
             *  \param argc: Argument count.
             *  \param argv: Argument vector.
             *  \return Returns any error.
             */
            int run(int& argc, char**& argv);
        //====================
        // ATTRIBUTES
        //====================
        private:
            std::unique_ptr<APP::ISignalCapturer>      signalCapturerPtr_;
            std::unique_ptr<APP::IDevInspector>        devInspectorPtr_;
            std::unique_ptr<APP::IDocGenerator>        docGeneratorPtr_;
            std::unique_ptr<APP::IScreen>              screenPtr_;
            static constexpr unsigned long             LENGTH_VOLT_SIGNAL {1024};
            static constexpr unsigned int              M_VALUES_TO_SHIFT  {1};
            inline static DOMN::VoltageSignal          voltages_[4] {
                DOMN::VoltageSignal(LENGTH_VOLT_SIGNAL, DOMN::SIGNAL_COLOR::RED),
                DOMN::VoltageSignal(LENGTH_VOLT_SIGNAL, DOMN::SIGNAL_COLOR::ORANGE),
                DOMN::VoltageSignal(LENGTH_VOLT_SIGNAL, DOMN::SIGNAL_COLOR::GREEN),
                DOMN::VoltageSignal(LENGTH_VOLT_SIGNAL, DOMN::SIGNAL_COLOR::BLUE),
            };
     };
}
