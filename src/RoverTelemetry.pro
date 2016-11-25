QT += core network positioning
QT -= gui

CONFIG += c++11

TARGET = RoverTelemetry
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    listener.cpp \
    sender.cpp \
    teledata.cpp \
    registry.cpp \
    imucomm.cpp \
    telemetry.cpp \
    datagrambuilder.cpp \
    gpscomm.cpp \
    restcomm.cpp

HEADERS += \
    listener.h \
    sender.h \
    teledata.h \
    registry.h \
    imucomm.h \
    telemetry.h \
    datagrambuilder.h \
    gpscomm.h \
    restcomm.h

INCLUDEPATH += ../3rdparty/QtWebApp

unix:!mac: {
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/IMUDrivers
LIBS += -lRTIMULib
DEFINES += USE_RTIMULib

target.path = /usr/local/bin
INSTALLS += target
}

win32: {
DEFINES += WIN32=1
DEFINES += _USE_MATH_DEFINES
#include(../3rdparty/RTIMULib/RTIMULib/RTIMULib.pri)
}

# Directory where the debug version of the shared library (*.dll or *.so) is stored, and base name of the file.
CONFIG(debug, debug|release) {
    win32:      LIBS += -L$$PWD/../3rdparty/lib/win32-msvc2013/debug/          -lQtWebAppd1
    unix:!mac:  LIBS += -L$$PWD/3rdparty/lib/rpi-g++/debug/              -lQtWebAppd
}

include(../common.pri)
