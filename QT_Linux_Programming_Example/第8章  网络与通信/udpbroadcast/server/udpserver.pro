TEMPLATE	= app
TARGET		= udpserver

CONFIG		+= qt warn_on release
DEPENDPATH	= ../../include

HEADERS		= udpserver.h
SOURCES		= udpserver.cpp \
		  main.cpp
TRANSLATIONS  = udpserver_zh.ts
QT += network
DESTDIR=..\..\Bin
MOC_DIR=..\..\Temp\udpbroadcast\server
RCC_DIR=..\..\Temp\udpbroadcast\server
UI_DIR=..\..\Temp\udpbroadcast\server
OBJECTS_DIR=..\..\Temp\udpbroadcast\server
CONFIG( debug, debug|release ){
DESTDIR=..\..\Bin\Debug
BUILD_NAME = debug
}