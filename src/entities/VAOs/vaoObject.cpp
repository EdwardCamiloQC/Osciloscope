#include <iostream>
#include <epoxy/gl.h>

#include <entities/VAOs/vaoObject.hpp>

using namespace VAOS_OBJECTS;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
bool VaoObject::createVAO(SignalObject &signalObject){
    glGenVertexArrays(1, &signalObject.vao_);
    glGenBuffers(2, signalObject.vbo_);

    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, 2*signalObject.length_*sizeof(float), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo_[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*signalObject.length_*sizeof(float), signalObject.colorVertex_, GL_STATIC_DRAW);

    glBindVertexArray(signalObject.vao_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo_[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo_[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OpenGL Error in createVAO: " << error << std::endl;
        return true;
    }

    return false;
}

bool VaoObject:: deleteVAO(SignalObject &signalObject){
    glDeleteVertexArrays(1, &signalObject.vao_);
    glDeleteBuffers(2, signalObject.vbo_);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OpenGL Error in deleteVAO: " << error << std::endl;
        return true;
    }

    return false;
}

bool VaoObject:: drawVAO(SignalObject &signalObject){
    glBindVertexArray(signalObject.vao_);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo_[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*signalObject.length_*sizeof(float), signalObject.vertex_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glLineWidth(2);
    glBindVertexArray(signalObject.vao_);
    glDrawArrays(GL_LINE_STRIP, 0, signalObject.length_);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OpenGL Error in drawVAO: " << error << std::endl;
        return true;
    }

    return false;
}