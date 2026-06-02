/** \file screen.hpp
 *  \author Edward Camilo.
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <memory>
#include "application/IScreen.hpp"

namespace APP{
    struct IGui;
    struct ISignalCapturer;
    struct IDevInspector;
    struct IDocGenerator;
}

/** \namespace INFRA.
 */
namespace INFRA{
    /** \class Screen.
     */
    class Screen: public APP::IScreen{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Principal Constructor disabled.
             */
            Screen() = delete;

            /** \brief Principal Constructor by default.
             *  \param gui: Specific graphic user interface.
             */
            [[nodiscard]] explicit Screen(unsigned int width, unsigned int heigth);

            /** \note Copy-constructor disabled.
             *  \param other: Screen object.
             */
            Screen(const Screen &other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: Screen object.
             */
            Screen(Screen &&other) = delete;

            /** \note Destructor by default.
             */
            ~Screen() = default;

            /** \note Assigment operator disabled.
             *  \param other: Screen object.
             *  \return Screen object.
             */
            Screen& operator =(const Screen &other) = delete;
            Screen& operator =(Screen &&other) = delete;

            /** \brief Associates the signal capturer with the Gui.
             *  \param signalCapturerPtr: Reference to the Signal Capturer.
             */
            void associate_signal_capturer(APP::ISignalCapturer* signalCapturerPtr)override final;

            /** \brief Associates the device inspector with the Gui.
             *  \param devInspPtr: Reference to the Dev Inspector.
             */
            void associate_dev_inspector(APP::IDevInspector* devInspPtr) override final;

            /** \brief Associates the doc generator with the Gui.
             *  \param docGenPtr: Reference to the Doc Generator.
             */
            void associate_doc_generator(APP::IDocGenerator* docGenPtr) override final;

            /** \brief Associates the voltages with the Gui.
             *  \param voltsPtr: Reference to the voltages.
             */
            void associate_voltages(DOMN::VoltageSignal* voltsPtr) override final;

            /** \brief Runs the program.
             *  \param argc: Argument count.
             *  \param argv: Argument vector.
             *  \return Returns the status of the application.
             */
            int turn_on(int &argc, char **&argv) override final;

            /** \brief Stops the program.
             *  \return Returns the status of the application.
             */
            int turn_off() override final;

            /** \brief Add the device to show it in the Gui.
             *  \param devName: Name of the device.
             */
            void add_device(const char* devName) override final;

            /** \brief Remove the device in the Gui.
             *  \param devName: Name of the device.
             */
            void remove_device(const char* devName) override final;

            /** \brief Updates the Button that shows the stateof the serial port.
             *  \param state: Open or close.
             */
            void update_port_state(bool state) override final;

            /** \brief Obtains the value of the time period to capture data.
             *  \return Time period.
             */
            long get_period_time_ns() override final;

            /** \brief Shows in the GUI the message.
             *  \param msg: Message.
             *  \param type: 0->in 1->standard 2->error 3->warning 4->open 5->close
             */
            void set_message(const char* msg, int type) const override final;
        //====================
        // ATRIBUTES
        //====================
        private:
            APP::IGui&    gui_;
            unsigned int  width_;
            unsigned int  heigth_;
    };
}
