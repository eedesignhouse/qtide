
android:{
          CONFIG += mobility
          MOBILITY +=
          TEMPLATE = lib
          TARGET = ../bin/qtide
          DEFINES += "ANDROID" }
else {    TEMPLATE = app
          TARGET = ../bin/jqt }
win32-msvc*:TARGET = ../bin/jqtx
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp
win32:config += console
CONFIG+= release

win32:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_LFLAGS +=
win32:RC_FILE = jqt.rc
