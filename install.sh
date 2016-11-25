#!/bin/sh
cd 3rdparty/qtposition_gpsd
qmake qtposition_gpsd.pro
make
sudo make install
cd ../RTIMULib
qmake RTIMULib
make
sudo make install
cd ../..
qmake RoverTelemetry.pro
make
sudo make install


