#include "image.hpp"

#include <iostream>

// Image::Image(.. some info.., unsigned char* data){
//
// construct Image class instance
//
//};

Image::~Image() {
  delete[] _data;
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

unsigned long Image::getWidth() {return _width;};
unsigned long Image::getHeight() { return _length;};
unsigned long Image::getChannels() { return _channels; };
unsigned long Image::getBpc() { return _bpc;};

