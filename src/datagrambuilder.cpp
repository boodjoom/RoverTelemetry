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

}

QByteArray DatagramBuilder::fromTeleData(TeleData *teleData)
{
    QJsonObject accel = toRawObject(teleData->imuData.accel);
    QJsonObject gyro = toRawObject(teleData->imuData.gyro);
    QJsonObject vehicle;
    vehicle["speed"]=QString::number(teleData->roverData.speed);
    vehicle["yaw"]=QString::number(teleData->roverData.yaw);

    QJsonObject telemetry;
    telemetry["accel"]=accel;
    telemetry["gyro"]=gyro;
    if(teleData->roverDataValid)
        telemetry["vehicle"]=vehicle;
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
