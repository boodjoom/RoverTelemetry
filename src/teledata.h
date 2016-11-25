#ifndef TELEDATA_H
#define TELEDATA_H
#include <QJsonObject>
#ifndef WIN32
#   define WIN32 1
#endif
#ifdef USE_RTIMULib
#   include "RTIMULib.h"
#else
#define RTFLOAT float
struct RTVector3{float data[3];float x(){return data[0];}float y(){return data[1];}float z(){return data[2];}};
#define MOTION_DATA int
#define RTMATH_RAD_TO_DEGREE 1
#endif
#include <QGeoCoordinate>

//RTFLOAT toDegrees(RTFLOAT& val)
//{
//    return (val * RTMATH_RAD_TO_DEGREE);
//}
struct ImuData
{
    RTVector3 accel;
    RTVector3 gyro;
    RTVector3 compas;
    RTVector3 fusionPose;
    RTFLOAT temperature;
    MOTION_DATA motionData;
    static RTFLOAT toDegrees(RTFLOAT val)
    {
        return (val * RTMATH_RAD_TO_DEGREE);
    }
};

struct RoverData
{
    double speed;
    double yaw;
    //enum {};
    int manip;
};

class TeleData
{
public:
    TeleData();
    ImuData imuData;
    bool imuDataValid;
    QGeoCoordinate geoData;
    bool geoDataValid;
    RoverData roverData;
    bool roverDataValid;
protected:

};

#endif // TELEDATA_H
