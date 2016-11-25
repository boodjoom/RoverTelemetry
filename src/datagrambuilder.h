#ifndef DATAGRAMBUILDER_H
#define DATAGRAMBUILDER_H

#include "teledata.h"
class QSettings;

class DatagramBuilder
{
public:
    DatagramBuilder(QSettings& settings);
    QByteArray fromTeleData(TeleData* teleData);
private:
    QJsonObject toAngleObject(RTVector3 vec);
    QJsonObject toRawObject(RTVector3 vec);
};

#endif // DATAGRAMBUILDER_H
