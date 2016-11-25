#ifndef GPSCOMM_H
#define GPSCOMM_H

#include <QObject>
#include "teledata.h"
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

class QSettings;

class GpsComm : public QObject
{
    Q_OBJECT
public:
    GpsComm(QSettings& settings, TeleData* teleData, QObject *parent = 0);

signals:

public slots:
    void start();
    void stop();
private slots:
    void satellitesInViewUpdated(const QList<QGeoSatelliteInfo>& infos);
    void satellitesInUseUpdated(const QList<QGeoSatelliteInfo>& infos);
    void error(QGeoSatelliteInfoSource::Error e);
    void positionUpdated(const QGeoPositionInfo &info);

private:
    void printSatellites(const QList<QGeoSatelliteInfo>& infos);
    void createSources();
    void destroySources();
    TeleData* _teleData;
    QGeoSatelliteInfoSource* _geoSatInfo;
    QGeoPositionInfoSource * _geoPosInfo;
    bool _logSatelites;
};

#endif // GPSCOMM_H
