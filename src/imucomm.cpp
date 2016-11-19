#include "imucomm.h"

ImuComm::ImuComm(TeleData *teleData, QObject *parent) : QObject(parent)
  ,_teleData(teleData)
{

}
