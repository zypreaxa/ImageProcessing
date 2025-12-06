#ifndef SIMPLE_IMAGE_LIBRARY_HPP
#define SIMPLE_IMAGE_LIBRARY_HPP

#include <string>
#include <vector>
#include <cstdint>

struct Pixel{
		uint8_t r, g, b;
	};

template <typename T>
class Image{
public:
	// Image constructors
	// Image(.. some info.., unsigned char* data); 
	Image(
	unsigned long width,
	unsigned long length,
	unsigned long channels,
	unsigned long bpc,
	T* data): _width(width), _length(length), _channels(channels), _bpc(bpc), _data(data)
	{}

	virtual ~Image(){
		delete[] _data;
	};

	// File related
	//virtual bool openFile() = 0; 
	unsigned long getWidth() {return _width;};
	unsigned long getHeight() { return _length;};
	unsigned long getChannels() { return _channels;};
	unsigned long getBpc() { return _bpc;};
	T* getImageData(){ return _data;};
	// get/set Pixel methods?
	
	Pixel getPixel(int x, int y) const {
		int idx = (y * _width + x) * _channels;
		Pixel p;
		p.r = static_cast<uint8_t>( _data[idx]);
		p.g = static_cast<uint8_t>(_data[idx+1]);
		p.b = static_cast<uint8_t>(_data[idx+2]);
		return p;
	};
	void setPixel(int x, int y, Pixel& p){
		int idx = (y * _width + x) * _channels;
		_data[idx] = static_cast<T>(p.r);
		_data[idx+1] = static_cast<T>(p.g);
		_data[idx+2] = static_cast<T>(p.b);
	}


	std::vector<unsigned int> getHistogram(){
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

	
protected:
	unsigned long _width{0};
	unsigned long _length{0};
	unsigned long _channels{0};
	unsigned long _bpc{0};

	std::string _filename{};
	
	T* _data{nullptr};
};

using Image8 = Image<uint8_t>;
#endif
