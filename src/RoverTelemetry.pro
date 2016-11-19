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
    registry.cpp

HEADERS += \
    listener.h \
    sender.h \
    teledata.h \
    registry.h
