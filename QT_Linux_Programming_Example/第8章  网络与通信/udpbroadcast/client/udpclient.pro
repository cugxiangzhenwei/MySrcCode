TEMPLATE	= app
TARGET		= udpclient

CONFIG		+= qt warn_on release
DEPENDPATH	= ../../include

HEADERS		= udpclient.h
SOURCES		= udpclient.cpp \
		  main.cpp
TRANSLATIONS  = udpclient_zh.ts
QT += network
DESTDIR=..\..\Bin
MOC_DIR=..\..\Temp\udpbroadcast\client
RCC_DIR=..\..\Temp\udpbroadcast\client
UI_DIR=..\..\Temp\udpbroadcast\client
OBJECTS_DIR=..\..\Temp\udpbroadcast\client
CONFIG( debug, debug|release ){
DESTDIR=..\..\Bin\Debug
BUILD_NAME = debug
}