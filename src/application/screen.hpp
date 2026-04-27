/** \file screen.hpp
 *  \author Edward Camilo.
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

#include <memory>

struct Gui;

namespace SCREEN{
    /** \class Screen.
     */
    class Screen{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Principal Constructor disabled.
             */
            [[nodiscard]] Screen() = delete;

            /** \brief Principal Constructor by default.
             *  \param gui: Specific graphic user interface.
             */
            [[nodiscard]] Screen(std::unique_ptr<Gui> &&gui, unsigned int width, unsigned int heigth);

            /** \note Copy-constructor disabled.
             *  \param other: Screen object.
             */
            [[nodiscard]] Screen(const Screen &other) = delete;

            /** \note Move-constructor disabled.
             *  \param other: Screen object.
             */
            [[nodiscard]] Screen(Screen &&other) = delete;

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
            void select_gui(std::unique_ptr<Gui> &&gui);

            /** \brief Runs the program.
             *  \param argc: Argument count.
             *  \param argv: Argument vector.
             *  \return Returns the status of the application.
             */
            int turn_on(int &argc, char **&argv);

            /** \brief Stops the program.
             *  \return Returns the status of the application.
             */
            int turn_off();

        //====================
        // ATRIBUTES
        //====================
        public:
            std::unique_ptr<Gui> gui_;
        private:
            unsigned int width_;
            unsigned int heigth_;
    };
}
