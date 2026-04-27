/** \file gridSpectrum.hpp
 *  \author Edward Camilo.
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

/** \brief The structure that contains the grid shape where the spectrum signals will be printed.
    \struct GridSpectrum.
 */
struct GridSpectrum{
    unsigned int vao, vbo[2]; ///< Vertex Array Object and Vertex Buffer Objects.
    static constexpr float vertex[32]={
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
    }; ///< Vertices belonging to the spectrum grid.

    static constexpr float color[48]={
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
    }; ///< Spectrum grid color.

    /** \brief Creates the spectrum grid.
     */
    void createGrid();

    /** \brief Deletes the spectrum grid.
     */
    void deleteGrid();

    /** \brief Draws the spectrum grid.
     */
    void draw();
};
