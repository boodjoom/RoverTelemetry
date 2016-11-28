#!/bin/sh
mkdir 3rdparty/build
cd 3rdparty/build
cmake ../RTIMULib/RTIMULib
make
sudo make install
cd ../..
qmake RoverTelemetry.pro
make
sudo make install


