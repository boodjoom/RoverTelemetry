#ifndef REGISTRY_H
#define REGISTRY_H

#include <QtCore>
#include <QHostAddress>
#include <teledata.h>


class Registry
{
public:
    Registry(QSettings& settings);
    void subscribe(const QHostAddress& host, quint16 port);
    void unsubscribe(const QHostAddress& host, quint16 port);
    const QSet< QPair< QHostAddress, quint16 > >& subscribers()const {return _subscribers;}
    void setTeleData(TeleData* teleData){_teleData=teleData;}
    TeleData* teleData(){return _teleData;}
private:
    QSet< QPair< QHostAddress, quint16 > > _subscribers;
    TeleData* _teleData;
};

#endif // REGISTRY_H
