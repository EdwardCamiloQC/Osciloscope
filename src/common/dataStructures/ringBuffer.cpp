#include <iostream>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "common/dataStructures/ringBuffer.hpp"

using namespace ED;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
RingBuffer::RingBuffer(size_t len):
    ringBufferPt_(nullptr),
    indicator_(0),
    length_(len)
{
    ringBufferPt_ = new float[length_];

    reset();
}

RingBuffer::~RingBuffer(){
    if(ringBufferPt_)
        delete[] ringBufferPt_;
}

void RingBuffer::reset(){
    for(size_t i = 0; i < length_; i++){
        ringBufferPt_[i] = 0.0f;
    }
}

void RingBuffer::push_data(const float *data, size_t w){
    if(w > length_)
        w = length_;

    if(indicator_ >= length_){
        indicator_ = 0; //regresa a la primera posición.
    }

    size_t firstPart = length_ - indicator_;
    if(w > firstPart){
        size_t i;
        for(i = 0; i < firstPart; i++){
            ringBufferPt_[indicator_] = data[i];
            indicator_++;
        }
        indicator_ = 0; //retoma en el inico del buffer.
        for(; i < w; i++){
            ringBufferPt_[indicator_]=  data[i];
            indicator_++;
        }
    }else{
        for(size_t i = 0; i < w; i++){
            ringBufferPt_[indicator_]= data[i];
            indicator_++;
        }
    }
}

void RingBuffer::push_data(const uint8_t* data, size_t w, uint8_t numBits, float voltRef){
    assert(this != nullptr);
    assert(ringBufferPt_ != nullptr);
    
    float resolution = voltRef/(pow(2, numBits-1) - 1);
    float averageVolt = pow(2, numBits)/2.0f;

    int inc;
    switch(numBits){
        case 8:
            inc = 1;
            break;
        case 10:
            inc = 2;
            break;
        case 12:
            inc = 2;
            break;
        case 16:
            inc = 2;
            break;
        default:
            inc = 1;
            break;
    }

    if(w > length_){//esta es la linea 84
        w = length_;
    }

    if(indicator_ >= length_){
        indicator_ = 0;
    }

    size_t sample;
    size_t firstPart = length_ - indicator_;
    if(w > firstPart){
        size_t secondPart = w - firstPart;
        if(inc == 1){
            for(sample = 0; indicator_ < length_; sample+=inc){
                ringBufferPt_[indicator_] = (static_cast<float>(static_cast<uint8_t>(data[sample]))-averageVolt) * resolution;
                indicator_++;
            }
            indicator_ = 0;
            for(; indicator_ < secondPart; sample+=inc){
                ringBufferPt_[indicator_] =  static_cast<float>(static_cast<uint8_t>(data[sample])-averageVolt) * resolution;
                indicator_++;
            }
        }else{
            for(sample = 0; indicator_ < length_; sample+=inc){
                ringBufferPt_[indicator_] = static_cast<float>(static_cast<uint16_t>(data[sample+1]<<8 | data[sample])-averageVolt) * resolution;
                indicator_++;
            }
            indicator_ = 0;
            for(; indicator_ < secondPart; sample+=inc){
                ringBufferPt_[indicator_] =  static_cast<float>(static_cast<uint16_t>(data[sample+1]<<8 | data[sample])-averageVolt) * resolution;
                indicator_++;
            }
        }
    }else{
        firstPart = indicator_ + w;
        if(inc == 1){
            for(sample = 0; indicator_ < firstPart; sample+=inc){
                ringBufferPt_[indicator_] = static_cast<float>(static_cast<uint8_t>(data[sample])-averageVolt) * resolution;
                indicator_++;
            }
        }else{
            for(sample = 0; indicator_ < firstPart; sample+=inc){
                ringBufferPt_[indicator_] = static_cast<float>(static_cast<uint16_t>(data[sample+1]<<8 | data[sample])-averageVolt) * resolution;
                indicator_++;
            }
        }
    }
}

void RingBuffer::get_n_data(float *bufStore, size_t n, unsigned int jump){
    if((n * jump) > length_)
        n = length_;

    if(indicator_ >= length_)
        indicator_ = 0;

    long iter = indicator_ - 1;
    long i = n - 1;
    for(; i >= 0; i--){
        if(iter < 0)
            iter = length_ - 1;
        bufStore[i] = ringBufferPt_[iter];
        iter-=jump;
    }
}

void RingBuffer::show_data() const{
    std::cout << "Ring Buffer: ";
    for(size_t i = 0; i < length_; i++){
        std::cout << ringBufferPt_[i] << "|";
    }
    std::cout << std::endl;
}

size_t RingBuffer::get_lenght(){
    return length_;
}

float* RingBuffer::get_ring_buffer_ref(unsigned int i) const{
    if(i >= length_){
        i = length_ - 1;
    }

    return ringBufferPt_ + i;
}
