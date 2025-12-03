#include "imageprocessor.hpp"
#include <algorithm>
#include <iostream>

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
    unsigned long bpc = red->getBpc(); // this assumes that all 3 images are the same size
    unsigned long channels = 3;

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
};

Image* ImageProcessor::negationtr(Image* img){
    size_t width = img->getWidth();
    size_t height = img->getHeight();
    unsigned long channels = img->getChannels();
    unsigned long bpc = img->getBpc();

    if(channels==1){ // if image is greyscale
        std::cout << "Inverting greyscale image...\n";

        for(size_t y=0; y<height; ++y){
            for(size_t x=0; x<width; ++x){
                Pixel p = img->getPixel(x, y);
                p.r = 255 - p.r; // since red has the first index, it is the representation of grey in greyscale

                img->setPixel(x, y, p);            
            }
        }
        return img;

    }
    else if(channels=3){ // if simple 3 channel RGB
        std::cout << "Inverting RGB image...\n";
        
        for(size_t y=0; y<height; y++){
            for(size_t x=0; x<width; x++){
                Pixel p = img->getPixel(x, y);
                p.r = 255 - p.r;
                p.g = 255 - p.g;
                p.b = 255 - p.b;
                
                img->setPixel(x, y, p);
            }
        }
        return img;
    }

}

