#ifndef SIL_TIFF_LOADER_HPP
#define SIL_TIFF_LOADER_HPP

#include "imageloader.hpp"

#include <tiffio.h> // Note use of libtiff

#include <string>

class TiffLoader : public ImageLoader {
public:
	TiffLoader(std::string filename);
	virtual ~TiffLoader();

	// Image data related
	Image* loadImage();
	void printMetaData();

	unsigned long getWidth() const {return _width;}
	unsigned long getLength() const {return _length;}
	unsigned long getChannels() const {return _channels;}
	unsigned long getBPC() const {return _bpc;}

protected:
	// File related
	bool openFile();
	TIFF *_tiff{nullptr};

	// functions for loading different tiff layouts
	unsigned char* loadTiles();
	unsigned char* loadStrips();
	unsigned char* loadScanline();
	
	// Hold Tiff metadata here?

	unsigned long _width{0};
	unsigned long _length{0};
	unsigned long _channels{0};
	unsigned long _bpc{0};

	unsigned int _directory{0};
	unsigned int _dirCount{0};
	bool _isTiled{false};
	bool _isStriped{false};
	bool _isScanline{false};
};

#endif
