#include "datagrambuilder.h"
#include <QSettings>
#include <QJsonDocument>
#include <QDateTime>
#ifndef WIN32
#   define WIN32 1
#endif
#ifdef USE_RTIMULib
#include "RTIMULib.h"
#endif

DatagramBuilder::DatagramBuilder(QSettings &settings)
{
    Q_UNUSED(settings)
}

QByteArray DatagramBuilder::fromTeleData(TeleData *teleData)
{
    QJsonObject telemetry;

    if(teleData->imuDataValid)
    {
        telemetry["accel"]=toRawObject(teleData->imuData.accel);
        telemetry["gyro"]=toAngleObject(teleData->imuData.gyro);
        telemetry["temp"]=QString::number(teleData->imuData.temperature);
        telemetry["mag"]=toAngleObject(teleData->imuData.compas);
        telemetry["pos"]=toAngleObject(teleData->imuData.fusionPose);
        telemetry["heading"]=QString::number(ImuData::toDegrees(teleData->imuData.heading));
        telemetry["velocity"]=toRawObject(teleData->imuData.motionData.worldVelocity);
        telemetry["is_moving"]=teleData->imuData.motionData.motion;
    }

    if(teleData->roverDataValid)
    {
        QJsonObject vehicle;
        vehicle["speed"]=QString::number(teleData->roverData.speed);
        vehicle["yaw"]=QString::number(teleData->roverData.yaw);
        vehicle["travel"]=QString::number(teleData->roverData.travel);
        telemetry["vehicle"]=vehicle;
    }

    if(teleData->geoDataValid)
    {
        QJsonObject gps;
        gps["lat"] = QString::number(teleData->geoData.latitude());
        gps["lon"] = QString::number(teleData->geoData.longitude());
        gps["alt"] = QString::number(teleData->geoData.altitude());
        telemetry["gps"] = gps;
    }
    telemetry["timestamp"]=QString::number(QDateTime::currentMSecsSinceEpoch());
    QJsonDocument doc;
    doc.setObject(telemetry);
    return doc.toJson();
}

QJsonObject DatagramBuilder::toAngleObject(RTVector3 vec)
{
    QJsonObject tmp;
    tmp["x"]=QString::number(ImuData::toDegrees(vec.x()));
    tmp["y"]=QString::number(ImuData::toDegrees(vec.y()));
    tmp["z"]=QString::number(ImuData::toDegrees(vec.z()));
    return std::move(tmp);
}

QJsonObject DatagramBuilder::toRawObject(RTVector3 vec)
{
    QJsonObject tmp;
    tmp["x"]=QString::number(vec.x());
    tmp["y"]=QString::number(vec.y());
    tmp["z"]=QString::number(vec.z());
    return std::move(tmp);
}
