#include "imageprocessor.hpp"
#include <cmath>
#include <iostream>

Image<>* ImageProcessor::combineRGB(
    Image<>* red,
    Image<>* green,
    Image<>* blue
)
{
    if (!red | !green | !blue) {
        return nullptr;
    } // checking for errors

    unsigned char width = red->getWidth();
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

    Image<>* out = new Image<>(width, height, channels, bpc, data);
    return out;
};

Image<>* ImageProcessor::togreyscale(Image<>* img){
    size_t width, height;
    unsigned long bpc, channels;
    width = img->getWidth();
    height = img->getHeight();
    channels = img->getChannels();
    uint8_t gsc;
    if(channels != 3 && channels != 1){
        std::cout << "This image is not a valid RGB format.";
        return nullptr;
    }
    else if(channels == 1){
        std::cout << "This image is already greyscale.";
        return nullptr;
    }
    for(size_t y=0; y<height; y++){
        for(size_t x=0; x<width; x++){
            Pixel<uint8_t> p = img->getPixel(x, y);
            gsc = std::round((p.r + p.g + p.b) / 3.0);
            p.r = gsc;
            p.g = gsc;
            p.b = gsc;
            img->setPixel(x, y, p);
        }
    }
    return img;
};  

Image<>* ImageProcessor::negationtr(Image<>* img){
    size_t width = img->getWidth();
    size_t height = img->getHeight();
    unsigned long channels = img->getChannels();
    unsigned long bpc = img->getBpc();

    if(channels==1){ // if image is greyscale
        std::cout << "Inverting greyscale image...\n";

        for(size_t y=0; y<height; ++y){
            for(size_t x=0; x<width; ++x){
                Pixel<> p = img->getPixel(x, y);
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
                Pixel<> p = img->getPixel(x, y);
                p.r = 255 - p.r;
                p.g = 255 - p.g;
                p.b = 255 - p.b;
                
                img->setPixel(x, y, p);
            }
        }
        return img;
    }

};

Image<>* ImageProcessor::negationlut(Image<>* img){
    size_t width = img->getWidth();
    size_t height = img->getHeight();

    std::cout << "Inverting greyscale image...\n";

    uint8_t lut[256];
    for(int k=0; k<256; k++){
        lut[k] = 255 - k;
    }

    for(size_t y=0; y<height; ++y){
        for(size_t x=0; x<width; ++x){
            Pixel<> p = img->getPixel(x, y);
            p.r = lut[p.r];
            p.g = lut[p.r];
            p.b = lut[p.r];
            img->setPixel(x, y, p);
        }
    }
    return img;
};

Image<>* ImageProcessor::powerlawtr(Image<>* img){
    unsigned long width = img->getWidth();
    unsigned long height = img->getHeight();
    unsigned long channels = img->getChannels(); // supposed to be mainly greyscale, but might as well
    unsigned long bpc = img->getBpc();
    int gamma;
    std::cout << "Select gamma value: ";
    std::cin >> gamma;

    if(channels==1){
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                p.r = 255.0 * std::pow(p.r / 255.0, gamma);
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else if(channels==3){  // completely incorrect lol
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                p.r = p.r ^ gamma;
                p.g = p.g ^ gamma;
                p.b = p.b ^ gamma;
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else {
        std::cout << "Image is of unknown format.";
        return nullptr;
    }
};

Image<>* ImageProcessor::powerlawlut(Image<>* img){
    unsigned long width = img->getWidth();
    unsigned long height = img->getHeight();
    unsigned long channels = img->getChannels(); // supposed to be mainly greyscale, but might as well
    unsigned long bpc = img->getBpc();
    int gamma;
    std::cout << "Select gamma value: ";
    std::cin >> gamma;

    if(channels==1){
        uint8_t lut[256];
        for(int k=0; k<256; k++){
            lut[k] = 255.0 * std::pow(k / 255.0, gamma);
        }

        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                p.r = lut[p.r];
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else if(channels==3){  // completely incorrect lol
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                p.r = p.r ^ gamma;
                p.g = p.g ^ gamma;
                p.b = p.b ^ gamma;
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else {
        std::cout << "Image is of unknown format.";
        return nullptr;
    }
};

Image<>* ImageProcessor::lineartr(Image<>* img) {
    unsigned long width = img->getWidth();
    unsigned long height = img->getHeight();
    unsigned long channels = img->getChannels();
    double L = 255.0;
    double r1 = 3.0 * L / 8.0;
    double s1 = L / 8.0;
    double r2 = 5.0 * L / 8.0;
    double s2 = 7.0 * L / 8.0;

    if (channels == 1) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                Pixel<> p = img->getPixel(x, y);
                double r = p.r;
                double s;

                if (r <= r1) {
                    s = (s1 / r1) * r;
                }
                else if (r <= r2) {
                    double m = (s2 - s1) / (r2 - r1);
                    s = m * (r - r1) + s1;
                }
                else {
                    double m = (L - 1 - s2) / (L - 1 - r2);
                    s = m * (r - r2) + s2;
                }

                // Clamp
                s = clamp(s, 0.0, 255.0);
                p.r = (unsigned char)s;
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else {
        std::cout << "This format cannot be transformed yet. Please choose a greyscale image." << std::endl;
        return nullptr;
    }
};

Image<>* ImageProcessor::linearlut(Image<>* img) {
    unsigned long width = img->getWidth();
    unsigned long height = img->getHeight();
    unsigned long channels = img->getChannels();

    double L = 255.0;
    double r1 = 3.0 * L / 8.0;
    double s1 = L / 8.0;
    double r2 = 5.0 * L / 8.0;
    double s2 = 7.0 * L / 8.0;

    if (channels == 1) {
        uint8_t lut[256];
        for(int k=0; k<256; k++){
            double r = k;
            double s;

            if (r <= r1) {
                s = (s1 / r1) * r;
            }
            else if (r <= r2) {
                double m = (s2 - s1) / (r2 - r1);
                s = m * (r - r1) + s1;
            }
            else {
                double m = (L - 1 - s2) / (L - 1 - r2);
                s = m * (r - r2) + s2;
            }

            s = clamp(s, 0.0, 255.0);
            lut[k]= static_cast<uint8_t>(std::round(s));
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Pixel<> p = img->getPixel(x, y);
                p.r = lut[p.r];
                p.g = lut[p.r];
                p.b = lut[p.r];
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else {
        std::cout << "This format cannot be transformed yet. Please choose a greyscale image." << std::endl;
        return nullptr;
    }
};

Image<>* ImageProcessor::thresholdtr(Image<>* img){
    unsigned long width, height, channels;
    width = img->getWidth();
    height = img->getHeight();
    channels = img->getChannels();

    int L, r0, s0, r1, s1, r2, s2, r3, s3, newval;
    L=256;
    r0=0; s0=r0;
    r1=L/2; s1=0;
    r2=r1; s2=L-1;
    r3=s2; s3=s2;

    if(channels==1){
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                if(p.r <= r1){
                    newval=s0+((s1-s0)/(r1-r0))*(p.r - r0);
                } 
                else if(p.r <= r2){
                    newval=s1+((s2-s1)/(r2-r1))*(p.r - r1);
                }
                else{
                    newval=s3+((s3-s2)/(r3-r2))*(p.r - r2);
                }
                clamp(newval, 0, 255);
                p.r = (unsigned char) newval;
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else{
        std::cout << "This format cannot be transformed yet. Please choose a greyscale image." << std::endl;
        return nullptr;
    }
};

Image<>* ImageProcessor::thresholdlut(Image<>* img){
    unsigned long width, height, channels;
    width = img->getWidth();
    height = img->getHeight();
    channels = img->getChannels();

    int L, r0, s0, r1, s1, r2, s2, r3, s3, newval;
    L=256;
    r0=0; s0=r0;
    r1=L/2; s1=0;
    r2=r1; s2=L-1;
    r3=s2; s3=s2;

    uint8_t lut[256];
    for(int k=0; k<256; k++){
        if(k <= r1){
            newval=s0+((s1-s0)/(r1-r0))*(k - r0);
        } 
        else if(k <= r2){
            newval=s1+((s2-s1)/(r2-r1))*(k - r1);
        }
        else{
            newval=s3+((s3-s2)/(r3-r2))*(k - r2);
        }
        clamp(newval, 0, 255);
        lut[k] = static_cast<uint8_t>(newval);
    }

    if(channels==1){
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                Pixel<> p = img->getPixel(x, y);
                p.r = lut[p.r];
                p.g = lut[p.r];
                p.b = lut[p.b];
                img->setPixel(x, y, p);
            }
        }
        return img;
    }
    else{
        std::cout << "This format cannot be transformed yet. Please choose a greyscale image." << std::endl;
        return nullptr;
    }
};

Image<>* ImageProcessor::histogramtr(Image<>* img){
    std::vector<unsigned int> hist = img->getHistogram();
    unsigned long width, height, x, y;
    width = img->getWidth();
    height = img->getHeight();
    double p[256];
    double res = width * height;

    for(int j=0; j<256; j++){
        p[j] = hist[j] / res;
    }
    double cdf[256];
    cdf[0] = p[0];

    for(int k=1; k<256; k++){
        cdf[k] = cdf[k-1] + p[k];
    }

    uint8_t lut[256];
    for(int k=0; k<256; k++){
        lut[k] = std::round((256-1)*cdf[k]);
    }

    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            Pixel<> p = img->getPixel(x, y);
            p.r = lut[p.r];
            p.g = lut[p.r];
            p.b = lut[p.r];
            img->setPixel(x, y, p);
        }
    }
    return img;
};

Image<float>* ImageProcessor::toFloat(Image<uint8_t>* img8) {
    size_t width = img8->getWidth();
    size_t height = img8->getHeight();
    size_t channels = img8->getChannels();
    unsigned long bpc = img8->getBpc(); 

    size_t nSamples = width * height * channels;

    uint8_t* src = img8->getImageData();
    float* dst = new float[nSamples];

    for (size_t i = 0; i < nSamples; ++i) {
        dst[i] = static_cast<float>(src[i]) / 255.0f;
    }

    unsigned long floatBpc = 32;
    return new Image<float>(width, height, channels, floatBpc, dst);
}

Image<uint8_t>* ImageProcessor::to8bit(Image<float>* imgf) {
    size_t width    = imgf->getWidth();
    size_t height   = imgf->getHeight();
    size_t channels = imgf->getChannels();
    size_t nSamples = width * height * channels;

    float* src = imgf->getImageData();
    uint8_t* dst = new uint8_t[nSamples];

    for (size_t i = 0; i < nSamples; ++i) {
        float v = clamp(src[i], 0.0f, 1.0f);
        int rounded = static_cast<int>(std::round(v * 255.0f));
        dst[i] = static_cast<uint8_t>(clamp(rounded, 0, 255));
    }

    unsigned long outBpc = 8;
    return new Image<uint8_t>(width, height, channels, outBpc, dst);
}

Image<uint8_t>* ImageProcessor::lowpass(Image<uint8_t>* img)
{
    size_t width    = img->getWidth();
    size_t height   = img->getHeight();
    size_t channels = img->getChannels();

    if (channels != 1) {
        std::cerr << "Error: lowpass() expects a grayscale image (1 channel)\n";
        return nullptr;
    }

    // Convert to float first
    Image<float>* imgf = toFloat(img);

    // Allocate output float image
    float* outBuf = new float[width * height]; // cause greyscale
    Image<float>* outFloat = new Image<float>(width, height, 1, 32, outBuf);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {

            float sum = 0.0f;
            int count = 0;

            // 3×3 neighborhood
            for (int dy = -1; dy <= 1; ++dy) {
                int ny = int(y) + dy;
                if (ny < 0 || ny >= int(height)) continue;

                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = int(x) + dx;
                    if (nx < 0 || nx >= int(width)) continue;

                    Pixel<float> p = imgf->getPixel(nx, ny);
                    sum += p.r;       
                    count++;
                }
            }

            Pixel<float> outPix;
            outPix.r = sum / float(count);

            outFloat->setPixel(x, y, outPix);
        }
    }

    Image<uint8_t>* result = to8bit(outFloat);

    delete imgf;
    delete outFloat;

    return result;
};