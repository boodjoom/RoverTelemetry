#ifndef IMUCOMM_H
#define IMUCOMM_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <teledata.h>
#include <QDebug>

class ImuComm : public QObject
{
    Q_OBJECT
public:
    ImuComm(TeleData* teleData, QObject *parent = 0);
    void setImuSettingsDir(const QString& dir)
    {
        qDebug()<<"Using Imu Settings in "<<dir;
        _settingsDir = dir;
    }
signals:
    void publish();
    void finished();
public slots:
    void start()
    {
        _mustExit=false;
        QMetaObject::invokeMethod(this,"run");
    }
    void stop()
    {
        _mustExit=true;
    }
protected:
    Q_SLOT void run();
    TeleData* _teleData;
    bool _mustExit;
    QString _settingsDir;
};

#endif // IMUCOMM_H
