#include "sender.h"
#include <QSettings>
#include <QTimer>
#include <QUdpSocket>

Sender::Sender(QSettings &settings, QObject *parent):QObject(parent)
  ,_timer(nullptr)
{
//    _timerPeriod = settings.value("Period",1000);
}

void Sender::start()
{
//    if(_timer)
//    {
//        qDebug()<<"Allready started";
//        return;
//    }
//    _timer = new QTimer(this);
//    connect(_timer,SIGNAL(timeout()),this,SLOT(publish()));
//    _timer->start(_timerPeriod);
}

void Sender::publish()
{
}
