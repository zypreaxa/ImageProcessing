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

std::vector<unsigned int> Image::getHistogram(){
	return std::vector<unsigned int>(256,0);
}

unsigned long Image::getWidth() {return _width;};
unsigned long Image::getHeight() { return _length;};
unsigned long Image::getChannels() { return _channels; };
unsigned long Image::getBpc() { return _bpc;};

