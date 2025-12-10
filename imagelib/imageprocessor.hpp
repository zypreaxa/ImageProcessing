#ifndef SIL_IMAGE_PROCESSOR_HPP
#define SIL_IMAGE_PROCESSOR_HPP

#include "image.hpp"
#include <string>
#include <array>

class ImageProcessor {
public: 

    static Image<>* combineRGB(
        Image<>* red,
        Image<>* green,
        Image<>* blue
    );
    
    static Image<>* togreyscale(Image<>* img);
    static Image<>* negationtr(Image<>* img);
    static Image<>* negationlut(Image<>* img);
    static Image<>* powerlawtr(Image<>* img);
    static Image<>* powerlawlut(Image<>* img);
    static Image<>* lineartr(Image<>* img);
    static Image<>* linearlut(Image<>* img);
    static Image<>* thresholdtr(Image<>* img);
    static Image<>* thresholdlut(Image<>* img);
    static Image<>* histogramtr(Image<>* img);
    static Image<>* lowpass(Image<>* img);    
};

#endif