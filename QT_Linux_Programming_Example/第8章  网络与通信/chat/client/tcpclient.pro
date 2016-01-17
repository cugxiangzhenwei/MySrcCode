TEMPLATE	= app
TARGET		= tcpclient

CONFIG		+= qt warn_on release
DEPENDPATH	= ../../include

HEADERS		= tcpclient.h
SOURCES		= tcpclient.cpp \
		  main.cpp
TRANSLATIONS  = tcpclient_zh.ts
QT += network
DESTDIR=..\..\Bin
MOC_DIR=..\..\Temp\chat\client
RCC_DIR=..\..\Temp\chat\client
UI_DIR=..\..\Temp\chat\client
OBJECTS_DIR=..\..\Temp\chat\client
CONFIG( debug, debug|release ){
DESTDIR=..\..\Bin\Debug
BUILD_NAME = debug
}