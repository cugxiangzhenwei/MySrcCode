TEMPLATE	= app
TARGET		= fileinformation

CONFIG		+= qt warn_on release
DEPENDPATH	= ../../include

HEADERS		= fileinformation.h
SOURCES		= fileinformation.cpp \
		  main.cpp
TRANSLATIONS  = fileinformation_zh.ts
DESTDIR=..\..\Bin
MOC_DIR=..\..\Temp\FormMoc
RCC_DIR=..\..\Temp\FormMoc
UI_DIR=..\..\Temp\FormMoc
OBJECTS_DIR=..\..\Temp
CONFIG( debug, debug|release ){
DESTDIR=..\..\Bin\Debug
BUILD_NAME = debug
}