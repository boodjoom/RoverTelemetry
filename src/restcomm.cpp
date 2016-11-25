#include "restcomm.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>

RestComm::RestComm(QSettings &settings, TeleData *teleData, QObject *parent)
    : QObject(parent)
  , _teleData(teleData)
  , _started(false)
{
    _roverHost = settings.value("RoverHost","127.0.0.1").toString();
    bool ok;
    _roverPort = settings.value("RoverPort",8080).toUInt(&ok);
    if(!ok)
        _roverPort = 8080;
    QObject::connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(&_requestTimer,SIGNAL(timeout()),this, SLOT(connect()));
    QObject::connect(&_socket,SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(&_socket,SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    QObject::connect(&_socket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onError(QAbstractSocket::SocketError)));
}

void RestComm::start()
{
    if(_started)
    {
        qDebug()<<"RestComm: Allready started";
        return;
    }
    _started = true;
    connect();
}

void RestComm::stop()
{
    if(_socket.state() == QAbstractSocket::ConnectedState)
    {
        _socket.disconnectFromHost();
    }
    _started = false;
}

void RestComm::connect()
{
    if(_socket.state() == QAbstractSocket::ConnectedState)
    {
        qDebug()<<"RestComm: Allready connected";
        connected();
        return;
    }
    qDebug()<<"RestComm: Try to connect";
    _socket.connectToHost(_roverHost,_roverPort);
}

void RestComm::connected()
{
    //_reconectTimer.stop();
    qDebug()<<"RestComm: Connected";
    request();
}

void RestComm::disconnected()
{
    //_reconectTimer.start(10000);
    qDebug()<<"RestComm: Disconnected";
    QTimer::singleShot(10000,this,SLOT(connect()));
}

void RestComm::onReadyRead()
{
    _rxBuffer = _socket.readAll();
    //qDebug()<<"RestComm: RxBuf: "<<_rxBuffer;
    stripHeader(_rxBuffer);
    QJsonDocument doc = QJsonDocument::fromJson(_rxBuffer);
    if(doc.isEmpty())
    {
        qDebug()<<"RestComm: no JSON data";
        return;
    }
    QJsonObject response = doc.object();
    handleResponse(response);
}

void RestComm::request()
{
    if(_socket.state() == QAbstractSocket::ConnectedState)
    {
        QString request = "GET /vehicle HTTP/1.1\r\nhost: "+
                _roverHost.toString()+"\r\n\r\n";
        QByteArray buffer;
        buffer.append(request);
        _socket.write(buffer);
        QTimer::singleShot(500,this,SLOT(request()));
    }
    else
        qDebug()<<"RestComm: requet on not connected state";
}

void RestComm::onError(QAbstractSocket::SocketError e)
{
    qDebug()<<"RestComm: SocketError "<< e;
    if(_socket.state() != QAbstractSocket::ConnectedState)
        QTimer::singleShot(10000,this,SLOT(connect()));
}

void RestComm::handleResponse(const QJsonObject &response)
{
    bool ok;
    double speed =0.0;
    bool speedUpdated=false;
    double yaw =0.0;
    bool yawUpdated=false;
    if(response.contains("speed"))
    {
        speed = response["speed"].toVariant().toDouble(&ok);
        if(ok)
        {
            qDebug()<<"RestComm: Speed received:"<<speed;
            speedUpdated=true;
        }
    }
    if(response.contains("yaw"))
    {
        yaw = response["yaw"].toVariant().toDouble(&ok);
        if(ok)
        {
            qDebug()<<"RestComm: Yaw received:"<<yaw;
            yawUpdated=true;
        }
    }
    if(speedUpdated)
        _teleData->roverData.speed=speed;
    if(yawUpdated)
        _teleData->roverData.yaw=yaw;
    _teleData->roverDataValid = yawUpdated || speedUpdated;
}

void RestComm::stripHeader(QByteArray &data)
{
    int i;
    int dataSize = data.size();
    for(i=0;i<dataSize-4;++i)
    {
        if(data[i]=='\r'
                && data[i+1]=='\n'
                && data[i+2]=='\r'
                && data[i+3] == '\n')
            break;
    }
    data = data.right(data.size()-i-4);
}
