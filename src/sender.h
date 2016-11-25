#ifndef SENDER_H
#define SENDER_H

#include <QObject>

class QSettings;
class QTimer;

class Sender:public QObject
{
    Q_OBJECT
public:
    Sender(QSettings& settings, QObject *parent = 0);
    void start();
    void stop();
private slots:
    void publish();
private:
    QTimer* _timer;
    quint32 _timerPeriod;
};

#endif // SENDER_H
