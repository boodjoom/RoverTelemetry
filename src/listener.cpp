#include "listener.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QSetIterator>

Listener::Listener(QSettings &settings, QObject *parent):
    QObject(parent)
  , _socket(nullptr)
  , _timer(nullptr)
  , _datagramBuilder(nullptr)
{
    bool ok;
    _bindPort = settings.value("Port",8888).toUInt(&ok);
    if(!ok)
        _bindPort=8888;
    _timerPeriod = settings.value("Period",1000).toUInt(&ok);
    if(!ok)
        _timerPeriod=1000;
}

void Listener::start()
{
    Q_ASSERT(!_registry.isNull());
    if(_socket)
    {
        qDebug()<<"Allready started";
        return;
    }
    _socket = new QUdpSocket(this);
    if(_socket)
    {
        _socket->bind(_bindPort);
        connect(_socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    }

    startPublish();
}

void Listener::stop()
{
    if(_socket)
    {
        delete _socket;
        _socket = nullptr;
    }
    if(_timer)
    {
        delete _timer;
        _timer = nullptr;
    }
}

void Listener::handleDatagram(const QByteArray& buffer, const QHostAddress& sender, quint16 senderPort)
{
    QJsonParseError err;
    QJsonDocument request = QJsonDocument::fromJson(buffer,&err);
    if(err.error != QJsonParseError::NoError)
    {
        qCritical()<<"Datagram parse error = "<<err.errorString();
    }
    if(request.isEmpty())
    {
        qCritical()<<"Datagram does not contain valid JSON data";
        return;
    }
    if(request.isArray())
    {
        qDebug()<<"Datagram contains array of objects";
        QJsonArray array = request.array();
        for(int i=0;i<array.size();++i)
        //Q_FOREACH(const QJsonObject& obj, array)
        {
            if(array[i].isObject())
                handleRequest(array[i].toObject(), sender, senderPort);
        }
    } else
        if(request.isObject())
        {
            handleRequest(request.object(), sender, senderPort);
        }
}

void Listener::handleRequest(const QJsonObject &request, const QHostAddress &sender, const quint16 senderPort)
{
    quint16 reqPort = senderPort;
    bool ok;
    QHostAddress reqHost = sender;
    if(request.contains("host"))
    {
        reqHost = request["host"].toString();
    }
    if(request.contains("port"))
    {
        reqPort = request["port"].toVariant().toUInt(&ok);
        if(!ok)
            reqPort = senderPort;
    }
    if(request.contains("action"))
    {
        const QString subscribe = "subscribe";
        const QString unsubscribe = "unsubscribe";
        const QString action = request["action"].toString();
        if(subscribe.compare(action,Qt::CaseInsensitive) == 0)
        {
            _registry->subscribe(reqHost ,reqPort);
        }
        else
        if(unsubscribe.compare(action,Qt::CaseInsensitive) == 0)
        {
            _registry->unsubscribe(reqHost ,reqPort);
        }
        else
           qCritical()<<"Request contains unknown action: "<<action;
    }
    else
    {
        qCritical()<<"Request does not contain action";
    }
}

void Listener::onReadyRead()
{
    // when data comes in
        QByteArray buffer;
        buffer.resize(_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
        //                 QHostAddress * address = 0, quint16 * port = 0)
        // Receives a datagram no larger than maxSize bytes and stores it in data.
        // The sender's host address and port is stored in *address and *port
        // (unless the pointers are 0).

        _socket->readDatagram(buffer.data(), buffer.size(),
                             &sender, &senderPort);

        qDebug() << "Message from: " << sender.toString();
        qDebug() << "Message port: " << senderPort;
        qDebug() << "Message: " << buffer;
        if(!buffer.isEmpty())
            handleDatagram(buffer, sender, senderPort);
}

void Listener::publish()
{
    if(!_registry->subscribers().size())
        return;
    qDebug()<<"Publish telemetry to "<<_registry->subscribers().size()<<" subscribers";
    QByteArray datagram = _datagramBuilder->fromTeleData(_registry->teleData());
    QSetIterator< QPair<QHostAddress,quint16> > iter(_registry->subscribers());
    iter.toFront();
    while(iter.hasNext())
    {
        const QPair<QHostAddress,quint16>& item = iter.next();
        _socket->writeDatagram(datagram,item.first,item.second);
    }
}

void Listener::startPublish()
{
    Q_ASSERT(_datagramBuilder);
    if(_timer)
    {
        qDebug()<<"Allready started";
        return;
    }
    _timer = new QTimer(this);
    connect(_timer,SIGNAL(timeout()),this,SLOT(publish()));
    _timer->start(_timerPeriod);
}
