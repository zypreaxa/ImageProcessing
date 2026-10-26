TEMPLATE = app
DEPENDPATH += ./
INCLUDEPATH += ./ ../ ../imagelib ## path to imagelib
RESOURCES += 

CONFIG += qt c++11 debug_and_release
QT += widgets gui charts

TARGET = viewer 
LIBS+= -L../imagelib/ -limagelib -ltiff -lfftw3f -Wl,-rpath,../imagelib

QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -g -O0
QMAKE_LFLAGS += -fsanitize=address

HEADERS += qtimageviewer.hpp

SOURCES += viewer.cpp \
	qtimageviewer.cpp 

## add call to compile image library
imagelib.target = myimagelibtarget
imagelib.commands = make -w -C ../imagelib/;
imagelib.depends =
QMAKE_EXTRA_TARGETS += imagelib
PRE_TARGETDEPS += myimagelibtarget
