#include "gpscomm.h"

#include <QSettings>
#include <QDebug>

#define log qDebug()<<"GpsComm: "

GpsComm::GpsComm(QSettings& settings, TeleData* teleData, QObject *parent) : QObject(parent)
  ,_teleData(teleData)
  , _geoSatInfo(nullptr)
  , _geoPosInfo(nullptr)
  , _logSatelites(false)
{
    Q_ASSERT(_teleData);
    _logSatelites = settings.value("LogSateliteInfo",true).toBool();
}

void GpsComm::start()
{
    log<<"starting...";
    createSources();
}

void GpsComm::stop()
{
    log<<"destroying...";
    destroySources();
    _teleData->geoDataValid=false;
}

void GpsComm::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &infos)
{
    log<<"satellitesInViewUpdated: "<<infos.size();
    printSatellites(infos);
}

void GpsComm::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &infos)
{
    log<<"satellitesInUseUpdated: "<<infos.size();
    printSatellites(infos);
}

void GpsComm::error(QGeoSatelliteInfoSource::Error e)
{
    qCritical()<<"GpsComm: error: "<<e;
}

void GpsComm::positionUpdated(const QGeoPositionInfo &info)
{
    log<<"positionUpdated "<<"longitude: "<<info.coordinate().longitude()
                  <<" latitude: "<<info.coordinate().latitude();
    _teleData->geoData = info.coordinate();
    _teleData->geoDataValid=true;
}

void GpsComm::printSatellites(const QList<QGeoSatelliteInfo> &infos)
{
    Q_FOREACH(const QGeoSatelliteInfo& info, infos)
    {
        QString attributes = "GpsComm:";
        if(info.hasAttribute(QGeoSatelliteInfo::Elevation))
        {
            attributes += QString(" elevation: %1").arg(info.attribute(QGeoSatelliteInfo::Elevation));
        }
        if(info.hasAttribute(QGeoSatelliteInfo::Azimuth))
        {
            attributes += QString(" azimuth: %1").arg(info.attribute(QGeoSatelliteInfo::Azimuth));
        }
                qDebug()<<"GpsComm: id: "<<info.satelliteIdentifier()<<" strenth: "<<info.signalStrength()
               <<attributes;
    }
}

void GpsComm::createSources()
{
    qDebug()<<"GpsComm: Start creating sources";
    if(_logSatelites){
        if(_geoSatInfo)
            qDebug()<<"GpsComm: SatelliteInfoSource allready exists";
        else
        {
        _geoSatInfo = QGeoSatelliteInfoSource::createDefaultSource(this);
        if(_geoSatInfo)
        {
            _geoSatInfo->setUpdateInterval(3000);
            connect(_geoSatInfo,SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
                    this, SLOT(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)));
            connect(_geoSatInfo, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
                    this, SLOT(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)));
            connect(_geoSatInfo, SIGNAL(error(QGeoSatelliteInfoSource::Error)), this, SLOT(error(QGeoSatelliteInfoSource::Error)));
            _geoSatInfo->startUpdates();
            qDebug()<<"GpsComm: SatelliteInfoSource '"<<_geoSatInfo->sourceName()<<"' created";
        }
        else
            qDebug()<<"GpsComm: SatelliteInfoSource create failed";
        }
    }

    if(_geoPosInfo)
        qDebug()<<"GpsComm: PositionInfoSource allready exists";
    else
    {
        _geoPosInfo = QGeoPositionInfoSource::createDefaultSource(this);
        if (_geoPosInfo) {
            connect(_geoPosInfo, SIGNAL(positionUpdated(QGeoPositionInfo)),
                    this, SLOT(positionUpdated(QGeoPositionInfo)));
            _geoPosInfo->startUpdates();
            _geoPosInfo->requestUpdate();
            qDebug()<<"GpsComm: PositionInfoSource '"<<_geoPosInfo->sourceName()<<"' created";
        }
        else
            qDebug()<<"GpsComm: PositionInfoSource create failed";
    }
}

void GpsComm::destroySources()
{
    if(_geoSatInfo)
    {
        delete _geoSatInfo;
        _geoSatInfo = nullptr;
    }
    if(_geoPosInfo)
    {
        delete _geoPosInfo;
        _geoPosInfo = nullptr;
    }
}
