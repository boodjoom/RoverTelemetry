#ifndef IMUCOMM_H
#define IMUCOMM_H

#include <QObject>
#include <teledata.h>

class ImuComm : public QObject
{
    Q_OBJECT
public:
    explicit ImuComm(QObject *parent = 0);

signals:

public slots:
protected:
    void run();
};

#endif // IMUCOMM_H
