#include <assert.h>
#include "domain/signals/signalObject.hpp"

using namespace DOMN;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
SignalObject::SignalObject(unsigned int len, VAO_COLOR_t color)
    : VaoObject(len, 0, color), length_(len)
{
    assign_color(color);
    organize_abscissas(length_);
    update_vertices(nullptr, 0.0f, 1.0f, length_);
}

void SignalObject::update_vertices(const float* signal, float offset, float voltDiv, unsigned int M){
    assert(M <= get_numOfPoints());

    if(M != length_){
        length_ = M;
        organize_abscissas(M);
    }

    if(signal == nullptr){
        for(unsigned int i = 1; i < (M*2); i+=2){
            verticesPtr_[i] = 0.0f;
        }
    }else{
        int ajust = 1;
        for(unsigned int i = 1; i < (M*2); i+=2){
            verticesPtr_[i] = 0.25f*(signal[i-ajust]+offset)/voltDiv;
            ajust++;
        }
    }
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================s
void SignalObject::organize_abscissas(unsigned int M){
    verticesPtr_[0] = -1.0f;
    for(unsigned int i = 2; i < (M*2); i+=2){
        verticesPtr_[i] = verticesPtr_[i-2] + static_cast<float>(2.0f/(M-1.0f));
    }
}

void SignalObject::assign_color(VAO_COLOR_t color){
    VaoObject::assign_color(color);
}
