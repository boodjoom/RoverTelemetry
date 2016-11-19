QT += core network
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
    telemetry.cpp

HEADERS += \
    listener.h \
    sender.h \
    teledata.h \
    registry.h \
    imucomm.h \
    telemetry.h

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
include(../3rdparty/RTIMULib/RTIMULib/RTIMULib.pri)
}

include(../common.pri)
