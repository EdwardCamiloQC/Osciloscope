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
            [[nodiscard]] explicit Screen(std::unique_ptr<APP::IGui> &&gui, unsigned int width, unsigned int heigth);

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

            /**
             * 
             */
            void select_gui(std::unique_ptr<APP::IGui> &&gui);

            /** \brief
             *  \param
             */
            void associate_signal_capturer(APP::ISignalCapturer* signalCapturerPtr)override final;

            /**
             * 
             */
            void associate_dev_inspector(APP::IDevInspector* devInspPtr) override final;

            /**
             * 
             */
            void associate_doc_generator(APP::IDocGenerator* docGenPtr) override final;

            /**
             * 
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

            /** \brief
             *  \param
             */
            void add_device(const char* devName) override final;

            /** \brief
             *  \param
             */
            void remove_device(const char* devName) override final;

            /**
             * 
             */
            void update_port_state(bool state) override final;

            /**
             * 
             */
            long get_period_time_ns() override final;

            /**
             * 
             */
            void set_message(const char* msg, int type) const override final;
        //====================
        // ATRIBUTES
        //====================
        private:
            std::unique_ptr<APP::IGui> guiPtr_;
            unsigned int               width_;
            unsigned int               heigth_;
    };
}
