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
    
    // image transforms, CA1 

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

    static Image<float>* toFloat(Image<uint8_t>* img8);
    static Image<uint8_t>* to8bit(Image<float>* imgf);

    static Image<>* lowpass(Image<>* img);  
    static Image<>* median(Image<>* img); 
    static Image<>* laplacian(Image<>* img);

private:
// my compiler is ancient so had to make my own clamp lol
    template <class T = uint8_t>
    static const T& clamp(const T& v, const T& lo, const T& hi)
    {
        return (v < lo) ? lo : (hi < v) ? hi : v;
    };

};

#endif