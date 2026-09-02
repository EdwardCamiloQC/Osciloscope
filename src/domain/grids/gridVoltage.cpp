#include "domain/grids/gridVoltage.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
GridVoltage::GridVoltage():
    VaoObject(sizeof(vertex_)/(2*sizeof(float)), 0, VAO_COLOR_t::GRAY)
{
    assign_vertices();
    assign_color(VAO_COLOR_t::GRAY);
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridVoltage::assign_color([[maybe_unused]]VAO_COLOR_t color){
    if(colorPtr_){
        for(unsigned int i = 0; i < sizeof(color_)/sizeof(float); i++){
            colorPtr_[i] = color_[i];
        }
    }
}

void GridVoltage::assign_vertices(){
    if(verticesPtr_){
        for(unsigned int i = 0; i < sizeof(vertex_)/sizeof(float); i++){
            verticesPtr_[i] = vertex_[i];
        }
    }
}
