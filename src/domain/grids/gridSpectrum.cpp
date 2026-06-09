#include "domain/grids/gridSpectrum.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
GridSpectrum::GridSpectrum():
    VaoObject(16, 0, VAO_COLOR_t::GRAY)
{
    assign_vertices();
    assign_color(VAO_COLOR_t::GRAY);
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridSpectrum::assign_color([[maybe_unused]]VAO_COLOR_t color){
    if(colorPtr_){
        for(unsigned int i = 0; i < 48; i++){
            colorPtr_[i] = color_[i];
        }
    }
}

void GridSpectrum::assign_vertices(){
    if(verticesPtr_){
        for(unsigned int i = 0; i < 32; i++){
            verticesPtr_[i] = vertex_[i];
        }
    }
}
