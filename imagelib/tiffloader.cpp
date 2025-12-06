#include "tiffloader.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>



TiffLoader::TiffLoader(std::string filename) : ImageLoader(filename){
	// ideally throw exceptions on fail, we skip that
	openFile();
};

TiffLoader::~TiffLoader() {};

bool TiffLoader::openFile(){
	std::cout<<"TiffLoader::OpenFile: "<<_filename<<std::endl;
	std::string fileExt{};

	if(getFileExt(fileExt) && (fileExt.compare(".tif") == 0) || (fileExt.compare(".tiff") == 0) || (fileExt.compare(".svs") == 0)) {

		std::cout<<"Opening image: "<<_filename<<std::endl;
		_tiff = TIFFOpen(_filename.c_str(),"r");
	
		if(_tiff){
			

			_dirCount = TIFFNumberOfDirectories(_tiff);
			std::cout<<"Tiff DIR count: "<< _dirCount << std::endl;
			if(_dirCount > 1 | _dirCount < 0) {
				int input;
				while(true) {
					std::cout << "This file contains multiple directories (0-"
					<< (_dirCount - 1) << "). Pick a directory: " << std::endl;
					std::cin >> input;

					// validation
					if(!std::cin) {
						std::cin.clear();
						//std::cin.ignore(10000, "\n");
						std::cout << "Error. Please select a valid directory index.\n";
						continue;
					}
					if(input < 0 || input >= _dirCount) {
						std::cout << "Directory does not exist. Please try again.\n";
						continue;
					}
					break; // valid choice hurray
				}
				
				std::cout << "Picked directory: " << input << std::endl;
				TIFFSetDirectory(_tiff, input);
			}

			if (TIFFIsTiled(_tiff)) { // moved down so it would not run directory 0 automatically
				_isTiled = true;
			}
			else { _isStriped = true;}

			// import necessary meta data
			TIFFGetField(_tiff, TIFFTAG_IMAGEWIDTH, &_width);
			TIFFGetField(_tiff, TIFFTAG_IMAGELENGTH, &_length);
			TIFFGetField(_tiff, TIFFTAG_SAMPLESPERPIXEL, &_channels);
			TIFFGetField(_tiff, TIFFTAG_BITSPERSAMPLE, &_bpc);

			std::cout<< std::left << std::setw(15) << "Width:" << 
			std::right << std::setw(10) << _width << std::endl;
		} 
		return true;
	};

	std::cout<<"Error loading as tif: "<<_filename<<std::endl;
	return false;
};

void TiffLoader::printMetaData() { // pretty n stuff
    std::cout << "+-----------------+------------+\n";
    std::cout << "| Field           | Value      |\n";
    std::cout << "+-----------------+------------+\n";
	
    std::cout << std::left << "| " << std::setw(15) << "Width"
    << " | " << std::right << std::setw(10) << _width << " |\n";

    std::cout << std::left << "| " << std::setw(15) << "Length"
    << " | " << std::right << std::setw(10) << _length << " |\n";

    std::cout << std::left << "| " << std::setw(15) << "Channels"
    << " | " << std::right << std::setw(10) << _channels << " |\n";

    std::cout << std::left << "| " << std::setw(15) << "Bits per sample"
    << " | " << std::right << std::setw(10) << _bpc << " |\n";

    std::cout << "+-----------------+------------+\n";
}

Image<uint8_t>* TiffLoader::loadImage(){
	// yes, should ideally throw an exception on fail, we skip that.
	if(_tiff){
		unsigned char* imageData{nullptr};
		if(_isStriped)
			imageData =  loadStrips();
		else if(_isTiled)
			imageData = loadTiles();
		else
			imageData = loadScanline();

		if(!imageData){
			return nullptr;
		}
		// construct and return an Image!
		// return new Image(...., imageData);
		return new Image(_width, _length, _channels, _bpc, imageData);
	}
	
	return nullptr;
};

unsigned char* TiffLoader::loadStrips(){
	std::cout<<"Image is: STRIPED"<<std::endl;
	// Check TiffReadEncodedStrip
	tstrip_t nStrips = TIFFNumberOfStrips(_tiff);
	tsize_t stripsize = TIFFStripSize(_tiff);
	unsigned char* buffer = new unsigned char[stripsize];
	unsigned char* data = new unsigned char[_width * _length * _channels * (_bpc / 8)];
	unsigned char* ptr = data;
	for(tstrip_t i=0; i<nStrips; i++){
		//actual code goes here
		tsize_t bytes = TIFFReadEncodedStrip(_tiff, i, buffer, -1);
		
		// error handling if bytesread=-1, may not be necessary?
		if(bytes==-1){
			delete[] buffer;
			delete[] data;
			return nullptr;
		}
		
		memcpy(ptr, buffer, bytes);
		ptr+=bytes;
	}
	delete[] buffer;
	return data;
};

unsigned char* TiffLoader::loadTiles(){
	std::cout<<"Image is: TILED" <<std::endl;
	// Check TiffReadEncodedTile / TiffReadTile

	ttile_t tilecount = TIFFNumberOfTiles(_tiff);
	ttile_t tilesize = TIFFTileSize(_tiff);

	uint32 tilewidth, tilelength;
	TIFFGetField(_tiff, TIFFTAG_TILEWIDTH, &tilewidth);
	TIFFGetField(_tiff, TIFFTAG_TILELENGTH, &tilelength);

	ttile_t tilesperrow = ceil((double)_width / tilewidth);
	ttile_t tilespercol = ceil((double)_length / tilelength);

	unsigned char* data = new unsigned char[_width * _length * _channels * (_bpc/8)];
	unsigned char* buffer = new unsigned char[tilesize];

	int bytesperpixel = (_channels * _bpc ) / 8;
	
	for(ttile_t t=0; t<tilecount; t++) {

		uint32 tileX = t % tilesperrow;
		uint32 tileY = t / tilesperrow;

		uint32 destX = tileX * tilewidth;
		uint32 destY = tileY * tilelength;

		tsize_t bytes = TIFFReadEncodedTile(_tiff, t, buffer, tilesize);

		if(bytes < 0) {
			delete[] buffer;
			delete[] data;
			std::cout << "Error while reading tile index " << t << "." << std::endl;
			return nullptr;
		}
		
		for(ttile_t i=0; i < tilelength; i++) {

			if(destY + i >= _length)
				break;

			// pointer to row INSIDE the tile
			unsigned char* tilerowptr = buffer + (i * tilewidth * bytesperpixel);

			// pointer to the row in the final image
			unsigned char* destrowptr = data + ((destY + i) * _width * bytesperpixel);
			destrowptr += destX * bytesperpixel;

			// this clips right edge if the tile overflows width (idk)
			uint32 copyWidth = std::min<uint32>(tilewidth, _width - destX);
			
			uint32 bytesToCopy = copyWidth * bytesperpixel;

			memcpy(destrowptr, tilerowptr, bytesToCopy);
		}

	}
	delete[] buffer;
	return data;
};
unsigned char* TiffLoader::loadScanline(){
	std::cout<<"TiffLoader::loadScanline() - TODO"<<std::endl;
	// Check TiffReadScanline
	return nullptr;
};
