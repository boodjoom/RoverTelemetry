#include "gpscomm.h"

GpsComm::GpsComm(TeleData* teleData, QObject *parent) : QObject(parent)
  ,_teleData(teleData)
{
    Q_ASSERT(_teleData);
}

void GpsComm::start()
{

}

void GpsComm::stop()
{

}
