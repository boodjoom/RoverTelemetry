#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QObject>

class Telemetry : public QObject
{
    Q_OBJECT
public:
    explicit Telemetry(QObject *parent = 0);

signals:

public slots:
};

#endif // TELEMETRY_H
