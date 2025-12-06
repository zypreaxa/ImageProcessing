#ifndef SIMPLE_IMAGE_LIBRARY_HPP
#define SIMPLE_IMAGE_LIBRARY_HPP

#include <string>
#include <vector>
#include <cstdint>

struct Pixel{
		uint8_t r, g, b;
	};

class Image{
public:
	// Image constructors
	// Image(.. some info.., unsigned char* data); 
	Image(
	unsigned long width,
	unsigned long length,
	unsigned long channels,
	unsigned long bpc,
	unsigned char* data): _width(width), _length(length), _channels(channels), _bpc(bpc), _data(data){}

	virtual ~Image();

	// File related
	//virtual bool openFile() = 0; 

	// Get attributes
	unsigned long getWidth();
	unsigned long getHeight();
	unsigned long getChannels();
	unsigned long getBpc();

	// Image data related
	unsigned char* getImageData();

	// get/set Pixel methods?
	
	Pixel getPixel(int x, int y) const {
		int idx = (y * _width + x) * _channels;
		Pixel p;
		p.r = _data[idx];
		p.g = _data[idx+1];
		p.b = _data[idx+2];
		return p;
	};
	void setPixel(int x, int y, Pixel& p){
		int idx = (y * _width + x) * _channels;
		_data[idx] = p.r;
		_data[idx+1] = p.g;
		_data[idx+2] = p.b;
	}


	// histogram - here using a std::vector
	std::vector<unsigned int> getHistogram();

	void toUChar();
	void toFloat();

protected:
	unsigned long _width{0};
	unsigned long _length{0};
	unsigned long _channels{0};
	unsigned long _bpc{0};

	std::string _filename{};
	
	unsigned char* _data{nullptr};
	float* _dataf{nullptr};
};
#endif
