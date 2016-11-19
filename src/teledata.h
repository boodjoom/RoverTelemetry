#ifndef TELEDATA_H
#define TELEDATA_H
#include <QJsonObject>
#ifndef WIN32
#   define WIN32 1
#endif
#include "RTIMULib.h"

struct ImuData
{
    RTVector3 accel;
    RTVector3 gyro;
    RTVector3 compas;
    RTVector3 fusionPose;
    RTFLOAT temperature;
    MOTION_DATA motionData;
    static RTFLOAT toDegrees(RTFLOAT& val)
    {
        return (val * RTMATH_RAD_TO_DEGREE);
    }
};

class TeleData
{
public:
    TeleData();
    ImuData imuData;
protected:

};

#endif // TELEDATA_H
