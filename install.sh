#!/bin/sh
echo "Install gpsd"
apt-get install gpsd
echo "Create service"
cp "service.sh" "/etc/init.d/RoverTelemetry"
chmod +x /etc/init.d/RoverTelemetry
echo "Install to run at boot time.."
update-rc.d RoverTelemetry defaults
update-rc.d gpsd defaults
