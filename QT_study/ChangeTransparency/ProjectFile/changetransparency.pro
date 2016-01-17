MY_VISON=1.0
TEMPLATE =app
TARGET=Transparency_$${MY_VISON}
DEST=../../Bin
DEPENDPATH+=. ../SourceCode
INCLUDEPATH+=.
#Input
HEADERS+=../SourceCode/changetransparency.h
SOURCES +=../SourceCode/changetransparency.cpp\
		../SourceCode/main.cpp