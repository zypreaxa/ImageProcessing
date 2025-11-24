#include "imageprocessor.hpp"
#include <algorithm>

Image* ImageProcessor::combineRGB(
    Image* red,
    Image* green,
    Image* blue
)
{
    if (!red | !green | !blue) {
        return nullptr;
    } // checking for errors

    unsigned long width = red->getWidth();
    unsigned long height = red->getHeight();
    unsigned long channels = red->getChannels();
    unsigned long bpc = red->getBpc(); // this assumes that all 3 images are the same size

    unsigned char* data = new unsigned char [width * height * channels * (bpc/8)];
    
    for (unsigned long y = 0; y < height; y++) {
        for (unsigned long x = 0; x < width; x++) {
            unsigned long idx = (y * width + x) * channels; // this turns the pixel coordinates from a 2D array into a 1D index
            data[idx] = red->getImageData()[y * width + x]; // R
            data[idx+1] = green->getImageData()[y * width + x]; // G
            data[idx+2] = blue->getImageData()[y * width + x]; // B
        }
    }

    Image* out = new Image(width, height, channels, bpc, data);
    return out;
}

