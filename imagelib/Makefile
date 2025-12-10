TARGET = libimagelib.so

.PHONY: all clean

all: $(TARGET)

imageloader.o: image.hpp imageloader.hpp imageloader.cpp
	g++ -fPIC -O2 imageloader.cpp -c

tiffloader.o: imageloader.o tiffloader.hpp tiffloader.cpp
	g++ -fPIC -O2 tiffloader.cpp -c

jpegloader.o: image.hpp jpegloader.hpp jpegloader.cpp
	g++ -fPIC -O2 jpegloader.cpp -c

imageprocessor.o: imageprocessor.o imageprocessor.hpp imageprocessor.cpp
	g++ -fPIC -O2 imageprocessor.cpp -c

$(TARGET): imageloader.o tiffloader.o jpegloader.o imageprocessor.o
	g++ -shared imageloader.o tiffloader.o jpegloader.o imageprocessor.o -o $(TARGET)

clean:
	rm *.o;
	rm $(TARGET)
