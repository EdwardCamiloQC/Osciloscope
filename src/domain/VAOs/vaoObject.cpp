#include "domain/VAOs/vaoObject.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
VaoObject::VaoObject(uint64_t numOfPoints, uint32_t indices, VAO_COLOR_t color):
    verticesPtr_(nullptr),
    colorPtr_(nullptr),
    elementsPtr_(nullptr),
    numOfPoints_(numOfPoints),
    numIndices_(indices)
{
    verticesPtr_     = new float[2*numOfPoints_];
    colorPtr_        = new float[3*numOfPoints_];

    if(numIndices_ > 2)
        elementsPtr_ = new unsigned int[numIndices_];

    assign_color(color);
}

VaoObject::VaoObject(VaoObject&& other) noexcept:
    verticesPtr_(other.verticesPtr_),
    colorPtr_(other.colorPtr_),
    elementsPtr_(other.elementsPtr_),
    numOfPoints_(other.numOfPoints_),
    numIndices_(other.numIndices_),
    vao_(other.vao_),
    vbo_{other.vbo_[0], other.vbo_[1], other.vbo_[2]}
{
    other.verticesPtr_ = nullptr;
    other.colorPtr_ = nullptr;
    other.elementsPtr_ = nullptr;
    other.vao_ = 0;
    other.vbo_[0] = 0;
    other.vbo_[1] = 0;
    other.vbo_[2] = 0;
}

VaoObject::~VaoObject(){
    if(verticesPtr_)
        delete[] verticesPtr_;
    
    if(colorPtr_)
        delete[] colorPtr_;
    
    if(elementsPtr_)
        delete[] elementsPtr_;
}

VaoObject& VaoObject::operator=(VaoObject&& other) noexcept{
    if(this != &other){
        delete[] verticesPtr_;
        delete[] colorPtr_;
        delete[] elementsPtr_;

        numOfPoints_ = other.numOfPoints_;
        numIndices_  = other.numIndices_;
        vao_         = other.vao_;
        vbo_[0]      = other.vbo_[0];
        vbo_[1]      = other.vbo_[1];
        vbo_[2]      = other.vbo_[2];
        verticesPtr_ = other.verticesPtr_;
        colorPtr_    = other.colorPtr_;
        elementsPtr_ = elementsPtr_;

        other.verticesPtr_ = nullptr;
        other.colorPtr_    = nullptr;
        other.elementsPtr_ = nullptr;
    }
    return *this;
}

float*  VaoObject::get_vertices_pt() const{
    return verticesPtr_;
}

float*  VaoObject::get_color_pt() const{
    return colorPtr_;
}

unsigned int*  VaoObject::get_elements_pt() const{
    return elementsPtr_;
}

uint64_t  VaoObject::get_numOfPoints() const{
    return numOfPoints_;
}

uint64_t  VaoObject::get_numOfInd() const{
    return numIndices_;
}

uint32_t& VaoObject::get_vao(){
    return vao_;
}

uint32_t& VaoObject::get_vertex_ind(){
    return vbo_[0];
}

uint32_t& VaoObject::get_color_ind(){
    return vbo_[1];
}

uint32_t& VaoObject::get_elements_ind(){
    return vbo_[2];
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PROTECTED METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void VaoObject::assign_color(VAO_COLOR_t color){
    float r, g, b;
    switch(color){
        case VAO_COLOR_t::RED:
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case VAO_COLOR_t::GREEN:
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case VAO_COLOR_t::BLUE:
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
            break;
        case VAO_COLOR_t::YELLOW:
            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case VAO_COLOR_t::ORANGE:
            r = 1.0f;
            g = 0.5f;
            b = 0.0f;
            break;
        case VAO_COLOR_t::GRAY:
            r = 0.5f;
            g = 0.5f;
            b = 0.5f;
            break;
        case VAO_COLOR_t::BLACK:
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case VAO_COLOR_t::WHITE:
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
    }

    for(unsigned int i = 0; i < 3*numOfPoints_; i+=3){
        colorPtr_[i]   = r;
        colorPtr_[i+1] = g;
        colorPtr_[i+2] = b;
    }
}

void VaoObject::assign_elements(){
}
