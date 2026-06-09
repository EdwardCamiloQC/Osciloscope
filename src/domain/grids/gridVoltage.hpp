/** \file gridVoltage.hpp
 *  \author Edward Camilo.
 *  \version v2.0
 *  \date June 2026
 */
#pragma once

#include "domain/VAOs/vaoObject.hpp"

/** \namespace DOMN.
 */
namespace DOMN{
    /** \brief The structure that contains the grid shape where the voltage signals will be printed.
        \class GridVoltage
    */
    class GridVoltage: public VaoObject{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             */
            [[nodiscard]] GridVoltage();

            /** \note Copy-Constructor disabled.
             *  \param other: GridVoltage.
             */
            GridVoltage(const GridVoltage& other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: GridVoltage.
             */
            GridVoltage(GridVoltage&& other) = delete;

            /** \note Destructor by default.
             */
            ~GridVoltage() = default;

            /** \note Assignment operator disabled.
             *  \param other: GridVoltage.
             *  \return GridVoltage.
             */
            GridVoltage& operator=(const GridVoltage& other) = delete;
            GridVoltage& operator=(GridVoltage&& other) = delete;
        private:
            /** \brief Assigns the color to the grid.
             *  \param color: Grid color.
             */
            void assign_color(VAO_COLOR_t color) override final;

            /** \brief Assignment vertices.
             */
            void assign_vertices();
        //====================
        // ATTRIBUTES
        //====================
        private:
            float vertex_[56]={
                //x1   y1     x2     y2
                -0.25f, +1.0f, -0.25f, -1.0f,
                -0.50f, +1.0f, -0.50f, -1.0f,
                -0.75f, +1.0f, -0.75f, -1.0f,
                +0.00f, +1.0f, +0.00f, -1.0f,
                +0.25f, +1.0f, +0.25f, -1.0f,
                +0.50f, +1.0f, +0.50f, -1.0f,
                +0.75f, +1.0f, +0.75f, -1.0f,
                //x1   y1    x2     y2
                -1.0f, -0.25f, +1.0f, -0.25f,
                -1.0f, -0.50f, +1.0f, -0.50f,
                -1.0f, -0.75f, +1.0f, -0.75f,
                -1.0f, +0.00f, +1.0f, +0.00f,
                -1.0f, +0.25f, +1.0f, +0.25f,
                -1.0f, +0.50f, +1.0f, +0.50f,
                -1.0f, +0.75f, +1.0f, +0.75f
            };

            float color_[84]={
                //R     G       B       R       G       B
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                //R     G       B       R       G       B
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f
            };
    };
}
