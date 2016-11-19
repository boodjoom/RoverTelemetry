#ifndef IMUCOMM_H
#define IMUCOMM_H

#include <QObject>
#include <teledata.h>

class ImuComm : public QObject
{
    Q_OBJECT
public:
    ImuComm(TeleData* teleData, QObject *parent = 0);

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
    void run();
    TeleData* _teleData;
    bool _mustExit;
};

#endif // IMUCOMM_H
