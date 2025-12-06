#include "image.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

// Image::Image(.. some info.., unsigned char* data){
//
// construct Image class instance
//
//};

Image::~Image() {
  delete[] _data;
  delete[] _dataf;
};

unsigned char* Image::getImageData(){
     return _data;
};

std::vector<unsigned int> Image::getHistogram()
{
    std::vector<unsigned int> histogram(256, 0);

    for (unsigned long y = 0; y < _length; ++y) {
        for (unsigned long x = 0; x < _width; ++x) {

            int idx = (y * _width + x);
            Pixel p = getPixel(x, y);

            histogram[p.r]++;
        }
    }
    return histogram;
};

void Image::toUChar()
{
    if(!_data){
        size_t size = _width * _length * _channels * _bpc /8;
        unsigned char* _data = new unsigned char[size];

        for (size_t i=0; i<size; ++i) {
            float f = _dataf[i];           
            float scaled = f * 255.0f;     
            int nval = std::round(scaled); 

            nval = std::clamp(nval, 0, 255); 
            _data[i] = static_cast<uint8_t>(nval);
        } 
        delete[] _dataf;
    } 
};
void Image::toFloat(){
    if(!_dataf){
        size_t size = _width * _length * _channels * _bpc / 8;
        float*_dataf = new float[size];
        for(size_t i=0; i<size; i++){
            uint8_t c = _data[i];
            _dataf[i] = c / 255.0f;         
        }
        delete[] _data;
    }
};

unsigned long Image::getWidth() {return _width;};
unsigned long Image::getHeight() { return _length;};
unsigned long Image::getChannels() { return _channels; };
unsigned long Image::getBpc() { return _bpc;};

