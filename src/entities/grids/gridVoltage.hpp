/** \file gridVoltage.hpp
 *  \author Edward Camilo.
 *  \version v1.0
 *  \date April 2026
 */
#pragma once

/** \brief The structure that contains the grid shape where the voltage signals will be printed.
    \struct GridVoltage
 */
struct GridVoltage{
    unsigned int vao, vbo[2]; ///< Vertex Array Object and Vertex Buffers Objects.
    float vertex[56]={
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
    }; ///< Vertices belonging to the signals grid.
    
    float color[84]={
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
    }; ///< Grid color.

    /** \brief Creates the signals grid.
     */
    void createGrid();

    /** \brief Deletes the signals grid.
     */
    void deleteGrid();

    /** \brief Draws the signals grid
     */
    void draw();
};
