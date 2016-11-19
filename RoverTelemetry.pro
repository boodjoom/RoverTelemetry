TEMPLATE  = subdirs

unix:!mac:SUBDIRS += 3rdparty/qtposition_gpsd/qtposition_gpsd.pro

SUBDIRS  += src/RoverTelemetry.pro

CONFIG   += ordered
