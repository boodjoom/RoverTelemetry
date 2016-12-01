TEMPLATE  = subdirs

SUBDIRS  = src/RoverTelemetry.pro

unix:!mac: SUBDIRS += 3rdparty/qtposition_gpsd/qtposition_gpsd.pro

win32: {
    SUBDIRS+=../RoverTelemetryClient
}

CONFIG   += ordered

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
