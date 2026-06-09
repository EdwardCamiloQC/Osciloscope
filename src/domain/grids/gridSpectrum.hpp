/** \file gridSpectrum.hpp
 *  \author Edward Camilo.
 *  \version v2.0
 *  \date June 2026
 */
#pragma once

#include "domain/VAOs/vaoObject.hpp"

/** \namespace DOMN.
 */
namespace DOMN{
    /** \brief The structure that contains the grid shape where the spectrum signals will be printed.
        \class GridSpectrum.
    */
    class GridSpectrum: public VaoObject{
        //====================
        // METHODS
        //====================
        public:
            /** \brief Constructor.
             */
            [[nodiscard]] GridSpectrum();

            /** \note Copy-Constructor disabled.
             *  \param other: GridSpectrum.
             */
            GridSpectrum(const GridSpectrum& other) = delete;

            /** \note Move-Constructor disabled.
             *  \param other: GridSpectrum.
             */
            GridSpectrum(GridSpectrum&& other) = delete;

            /** \note Destructor by default.
             */
            ~GridSpectrum() = default;

            /** \note Assignment operator disabled.
             *  \param other: GridSpectrum.
             *  \return GridSpectrum.
             */
            GridSpectrum& operator=(const GridSpectrum& other) = delete;
            GridSpectrum& operator=(GridSpectrum&& other) = delete;
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
            static constexpr float vertex_[32]={
                //x1    y1      x2      y2
                -0.25f, +1.0f, -0.25f, -1.0f,
                -0.50f, +1.0f, -0.50f, -1.0f,
                -0.75f, +1.0f, -0.75f, -1.0f,
                +0.00f, +1.0f, +0.00f, -1.0f,
                +0.25f, +1.0f, +0.25f, -1.0f,
                +0.50f, +1.0f, +0.50f, -1.0f,
                +0.75f, +1.0f, +0.75f, -1.0f,
                //x1    y1      x2      y2
                -1.0f, -1.0f,   1.0f,  -1.0f
            };

            static constexpr float color_[48]={
                //R     G       B       R       G       B
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                0.4f,   0.4f,   0.4f,   0.4f,   0.4f,   0.4f,
                //R     G       B       R       G       B
                0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f
            };
    };
}
