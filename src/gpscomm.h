#ifndef GPSCOMM_H
#define GPSCOMM_H

#include <QObject>
#include "teledata.h"

class GpsComm : public QObject
{
    Q_OBJECT
public:
    GpsComm(TeleData* teleData, QObject *parent = 0);

signals:

public slots:
    void start();
    void stop();
private slots:
private:
    TeleData* _teleData;
};

#endif // GPSCOMM_H
