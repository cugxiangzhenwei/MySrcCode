TEMPLATE	= app
TARGET		= tcpserver

CONFIG		+= qt warn_on release
DEPENDPATH	= ../../include

HEADERS		= tcpserver.h \
				tcpclientsocket.h \
				server.h
SOURCES		= tcpserver.cpp \
				tcpclientsocket.cpp \
				server.cpp \
		  main.cpp
TRANSLATIONS  = tcpserver_zh.ts
QT += network
DESTDIR=..\..\Bin
MOC_DIR=..\..\Temp\chat\server
RCC_DIR=..\..\Temp\chat\server
UI_DIR=..\..\Temp\chat\server
OBJECTS_DIR=..\..\Temp\chat\server
CONFIG( debug, debug|release ){
DESTDIR=..\..\Bin\Debug
BUILD_NAME = debug
}