#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QUdpSocket>

class Listener: public QObject
{
    Q_OBJECT
public:
    Listener(QObject* parent =0);
private:
    QUdpSocket _socket;
};

#endif // LISTENER_H
