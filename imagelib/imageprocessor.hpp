#ifndef SIL_IMAGE_PROCESSOR_HPP
#define SIL_IMAGE_PROCESSOR_HPP

#include "image.hpp"
#include <string>
template <typename T>
class Image;

class ImageProcessor {
public: 

    static Image<uint8_t>* combineRGB(
        Image<uint8_t>* red,
        Image<uint8_t>* green,
        Image<uint8_t>* blue
    );
    
    static Image<uint8_t>* negationtr(Image<uint8_t>* img);
    static Image<uint8_t>* negationlut(Image<uint8_t>* img);
    static Image<uint8_t>* powerlawtr(Image<uint8_t>* img);
    static Image<uint8_t>* powerlawlut(Image<uint8_t>* img);
    static Image<uint8_t>* lineartr(Image* img);
    static Image<uint8_t>* linearlut(Image* img);
    static Image<uint8_t>* thresholdtr(Image* img);
    static Image<uint8_t>* thresholdlut(Image* img);
    static Image<uint8_t>* histogramtr(Image* img);
     
};

#endif