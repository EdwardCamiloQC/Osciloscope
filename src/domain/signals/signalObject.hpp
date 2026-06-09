/** \file signalObject.hpp
    \author Edward Camilo
    \version v2.0
    \date June 2026
 */
#pragma once

#include "domain/VAOs/vaoObject.hpp"

namespace DOMN{
    /** \class SignalObject.
     */
    class SignalObject: public VaoObject{
        //====================
        // METHODS
        //====================
        public:
            /** \note Principal-Constructor disabled.
             */
            SignalObject() = delete;

            /** \brief Constructor.
             *  \param len: Max number of points that make up the signal.
             *  \param color: Signal Color.
             */
            [[nodiscard]] explicit SignalObject(unsigned int len, VAO_COLOR_t color);

            /** \note Copy-Constructor disabled.
             *  \param other: Other SignalObject.
             */
            SignalObject(const SignalObject& other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: Other SignalObject.
             */
            SignalObject(SignalObject&& other) = delete;

            /** \note Assignment operator disabled.
             *  \param other: Other SignalObject.
             *  \return Other SignalObject.
             */
            SignalObject& operator=(const SignalObject& other) = delete;
            SignalObject& operator=(SignalObject&& other) = delete;

            /** \brief Destructor by default.
             */
            ~SignalObject() = default;

            /** \brief Update the M first points in the buffer. The M points will be show it.
             *  \param signal: Buffer to update the vertices.
             *  \param offset: Offset.
             *  \param voltDiv: Volts per division.
             *  \param M: Amount data to update. (number of points to update).
             */
            void update_vertices(const float* signal, float offset, float voltDiv, unsigned int M);
        private:
            /** \brief Organizes the vertices correspond to de axe X.
             *  \param M: Amount of the points to organize according to display.
             */
            void organize_abscissas(unsigned int M);

            /** \brief Apply the color.
             *  \param color: Signal Color.
             */
            void assign_color(VAO_COLOR_t color) override final;
        //====================
        // ATTRIBUTES
        //====================
        public:
            unsigned int length_;
    };
}
