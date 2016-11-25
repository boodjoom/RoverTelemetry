#include "registry.h"
//#include <QMutableSetIterator>
#include <QSet>

Registry::Registry(QSettings &settings)
{
    Q_UNUSED(settings)
}

void Registry::subscribe(const QHostAddress &host, quint16 port)
{
    qDebug()<<"Add subscriber: "<<host.toString()<<":"<<port;
    _subscribers.insert(QPair<QHostAddress,quint16>(host, port));
}

void Registry::unsubscribe(const QHostAddress &host, quint16 port)
{
    qDebug()<<"Remove subscriber: "<<host.toString()<<":"<<port;
//    QMutableSetIterator< QPair< QHostAddress, quint16 > > iter(subscribers);
//    iter.toFront();
//    while(iter.hasNext())
//    {
//        const QPair< QHostAddress, quint16 >& subscriber = iter.next();
//        if(subscriber.first == host && subscriber.second == port)
//        {
//            iter.remove();
//        }
//    }
    _subscribers.remove(QPair<QHostAddress,quint16>(host, port));
}
