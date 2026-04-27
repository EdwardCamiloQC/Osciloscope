#include <iostream>

#include <common/dataStructures/ringBuffer.hpp>

using namespace ED;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
RingBuffer::RingBuffer(size_t len):
    ringBufferPt_(nullptr),
    endPt_(nullptr),
    indicatorPt_(nullptr),
    dataReturn_(nullptr),
    length_(len)
{
    ringBufferPt_ = new float[length_];
    endPt_ = &ringBufferPt_[length_ - 1];
    indicatorPt_ = ringBufferPt_;

    dataReturn_ = new float[length_];

    reset();
}

RingBuffer::~RingBuffer(){
    if(ringBufferPt_)
        delete[] ringBufferPt_;
    
    if(dataReturn_){
        delete[] dataReturn_;
    }
}

void RingBuffer::reset(){
    for(size_t i = 0; i < length_; i++){
        ringBufferPt_[i] = 0.0f;
    }
}

void RingBuffer::push_data(const float *data, size_t w){
    if(w > length_)
        w = length_;

    if(indicatorPt_ > endPt_){
        indicatorPt_ = ringBufferPt_;
    }

    size_t firstPart = endPt_ - indicatorPt_;
    if(w > firstPart){
        size_t i;
        for(i = 0; i <= firstPart; i++){
            *indicatorPt_ = data[i];
            indicatorPt_++;
        }
        indicatorPt_ = ringBufferPt_;
        for(; i < w; i++){
            *indicatorPt_=  data[i];
            indicatorPt_++;
        }
    }else{
        for(size_t i = 0; i < w; i++){
            *indicatorPt_= data[i];
            indicatorPt_++;
        }
    }
}

const float* RingBuffer::get_n_data(size_t n){
    if(n > length_)
        n = length_;

    if(indicatorPt_ > endPt_)
        indicatorPt_ = ringBufferPt_;

    float *iteratorPt = indicatorPt_ - 1;

    if(iteratorPt < ringBufferPt_)
        iteratorPt = endPt_;

    size_t secondPart = (iteratorPt - ringBufferPt_) + 1;

    long i = n - 1;

    if(n > secondPart){
        for(; iteratorPt >= ringBufferPt_; i--){
            dataReturn_[i] = *iteratorPt;
            iteratorPt--;
        }
        iteratorPt = endPt_;
        for(; i >= 0; i--){
            dataReturn_[i] = *iteratorPt;
            iteratorPt--;
        }
    }else{
        for(; i >= 0; i--){
            dataReturn_[i] = *iteratorPt;
            iteratorPt--;
        }
    }

    return dataReturn_;
}

void RingBuffer::show_data() const{
    std::cout << "Ring Buffer: ";
    for(size_t i = 0; i < length_; i++){
        std::cout << ringBufferPt_[i] << "|";
    }
    std::cout << std::endl;
}
