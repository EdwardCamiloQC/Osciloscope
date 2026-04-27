#include <epoxy/gl.h>

#include <entities/grids/gridVoltage.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridVoltage::createGrid(){
    //Genera el VAO y el VBO
    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(2, vbo);

    //Enlaza el VAO para configurar los atributos
    glBindVertexArray(vao);

    //Enlaza el VBO y copia los datos
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 56*sizeof(float), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 84*sizeof(float), color, GL_STATIC_DRAW);

    //Configurar atributos
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    //Desenlazar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Desenlazar el VAO
    glBindVertexArray(0);
}

void GridVoltage::deleteGrid(){
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
}

void GridVoltage::draw(){
    glLineWidth(1);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, sizeof(vertex)/(2*sizeof(float)));
    glBindVertexArray(0);
}
