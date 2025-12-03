#ifndef SIL_IMAGE_PROCESSOR_HPP
#define SIL_IMAGE_PROCESSOR_HPP

#include "image.hpp"
#include <string>

class Image;

class ImageProcessor {
public: 

    static Image* combineRGB(
        Image* red,
        Image* green,
        Image* blue
    );
    
    static Image* negationtr(
        Image* in
    );
     
};

#endif