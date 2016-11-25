#ifndef RESTCOMM_H
#define RESTCOMM_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>
#include "teledata.h"

class QSettings;

class RestComm : public QObject
{
    Q_OBJECT
public:
    RestComm(QSettings& settings, TeleData* teleData, QObject *parent = 0);

signals:

public slots:
    void start();
    void stop();
private slots:
    void connect();
    void connected();
    void disconnected();
    void onReadyRead();
    void request();
    void onError(QAbstractSocket::SocketError e);
private:
    TeleData* _teleData;
    QHostAddress _roverHost;
    quint32 _roverPort;
    QTcpSocket _socket;
    bool _started;
    QTimer _requestTimer;
    QByteArray _rxBuffer;
    void handleResponse(const QJsonObject& response);
    void stripHeader(QByteArray& data);
};

#endif // RESTCOMM_H
