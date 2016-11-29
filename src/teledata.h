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
typedef struct
{
    RTVector3 worldAcceleration;
    RTVector3 worldVelocity;
    RTVector3 worldPosition;
    RTVector3 worldVelocityDrift;
    RTVector3 residuals;
    bool motion;
} MOTION_DATA;
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
    RTFLOAT heading;
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
    int manipStateId;
    QString manipState;
    double travel;
    double battary;
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
