#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QUdpSocket>
#include <registry.h>
#include <datagrambuilder.h>

class Listener: public QObject
{
    Q_OBJECT
public:
    Listener(QSettings& settings, QObject* parent =0);
    void start();
    void stop();
    void setRegistry(QSharedPointer<Registry>& reg){_registry=reg;}
    void setDatagramBuilder(DatagramBuilder* datagramBuilder){_datagramBuilder=datagramBuilder;}
protected:
    void handleDatagram(const QByteArray& buffer, const QHostAddress& sender, quint16 senderPort);
    void handleRequest(const QJsonObject& request, const QHostAddress& sender, const quint16 senderPort);
private:
    QUdpSocket* _socket;
    quint16 _bindPort;
    QSharedPointer<Registry> _registry;
private slots:
    void onReadyRead();
    void publish();
private:
    QTimer* _timer;
    quint32 _timerPeriod;
    void startPublish();
    DatagramBuilder* _datagramBuilder;
};

#endif // LISTENER_H
