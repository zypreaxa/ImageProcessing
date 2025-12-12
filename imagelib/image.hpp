#ifndef SIMPLE_IMAGE_LIBRARY_HPP
#define SIMPLE_IMAGE_LIBRARY_HPP

#include <string>
#include <vector>
#include <cstdint>

class ImageBase {
public:
    virtual ~ImageBase() {}  

    virtual unsigned long getWidth() const = 0;
    virtual unsigned long getHeight() const = 0;
    virtual unsigned long getChannels() const = 0;
    virtual unsigned long getBpc() const = 0;

    virtual void* getRawData() = 0; 
};

template <typename T = uint8_t>
struct Pixel {
    T r, g, b;
};

// made this so we could just have Image<float> objects and stuff
template <typename T = uint8_t>
class Image : public ImageBase {
public:
    Image(unsigned long width,
          unsigned long height,
          unsigned long channels,
          unsigned long bpc,
          T* data)
        : _width(width), _height(height), _channels(channels), _bpc(bpc), _data(data) {}

    ~Image() override {
        delete[] _data;
    }

    unsigned long getWidth() const override { return _width; }
    unsigned long getHeight() const override { return _height; }
    unsigned long getChannels() const override { return _channels; }
    unsigned long getBpc() const override { return _bpc; }

    void* getRawData() override { return _data; };

    T* getImageData(){ return _data; }

    Pixel<T> getPixel(int x, int y) const {
        int idx = (y * _width + x) * _channels;
        Pixel<T> p{};
        p.r = _data[idx];
        p.g = _data[idx + 1];
        p.b = _data[idx + 2];
        return p;
    }

    void setPixel(int x, int y, const Pixel<T>& p) {
        int idx = (y * _width + x) * _channels;
        _data[idx]     = p.r;
        _data[idx + 1] = p.g;
        _data[idx + 2] = p.b;
    }

    std::vector<unsigned int> getHistogram() const {
        std::vector<unsigned int> histogram(256, 0);
        for (unsigned long y = 0; y < _height; ++y) {
            for (unsigned long x = 0; x < _width; ++x) {
                Pixel<uint8_t> p = getPixel(x, y);
                histogram[p.r]++;
            }
        }
        return histogram;
    };

protected:
    unsigned long _width{0};
    unsigned long _height{0};
    unsigned long _channels{0};
    unsigned long _bpc{0};

    std::string _filename{};

    T* _data{nullptr};
};

#endif
