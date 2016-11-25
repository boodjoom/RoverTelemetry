#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include "logging/filelogger.h"
#include "listener.h"
#include "datagrambuilder.h"
#include "imucomm.h"
#include "teledata.h"
#include "gpscomm.h"
#include "restcomm.h"

using namespace stefanfrings;

/** Logger class */
FileLogger* logger;

/** Search the configuration file */
QString searchConfigFile(const QString& appName, const QString& confName = QString())
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString fileName((confName.isEmpty()?appName:confName)+".ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../../etc"); // for development without shadow build
    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("RoverTelemetry");
    app.setOrganizationName("Shark Inc");

    // Find the configuration file
    QString configFileName=searchConfigFile(QCoreApplication::applicationName());

    // Configure logging into a file
    QSettings* loggerSettings = new QSettings(configFileName,QSettings::IniFormat,&app);
    loggerSettings->beginGroup("logger");
    FileLogger* logger = nullptr;
    if(loggerSettings->value("ToFile",false).toBool())
    {
        logger=new FileLogger(loggerSettings,10000,&app);
        logger->installMsgHandler();
    }


    QSettings settings(configFileName,QSettings::IniFormat,&app);

    qDebug()<<"Init telemetry data";
    TeleData* teleData = new TeleData();

    qDebug()<<"Init registry";
    settings.beginGroup("registry");
    QSharedPointer<Registry> registry = QSharedPointer<Registry>::create(settings);
    registry->setTeleData(teleData);
    settings.endGroup();

    qDebug()<<"Init imu source";
    ImuComm* imuComm = new ImuComm(teleData);
    imuComm->setImuSettingsDir(QFileInfo(configFileName).dir().canonicalPath());
    QThread* imuThread = new QThread(&app);
    imuComm->moveToThread(imuThread);
    QObject::connect(imuThread, SIGNAL(started()),imuComm, SLOT(start()),Qt::QueuedConnection);
    QObject::connect(imuThread, SIGNAL(finished()),imuThread, SLOT(deleteLater()),Qt::QueuedConnection);
    QObject::connect(imuComm, SIGNAL(finished()),imuComm, SLOT(deleteLater()),Qt::QueuedConnection);
    imuThread->start();

    qDebug()<<"Init gps source";
    settings.beginGroup("gps");
    GpsComm* gpsComm = new GpsComm(settings,teleData,&app);
    settings.endGroup();
    gpsComm->start();

    qDebug()<<"RoverRest source";
    settings.beginGroup("rover");
    RestComm* restComm = new RestComm(settings, teleData, &app);
    settings.endGroup();
    restComm->start();

    qDebug()<<"Init datagram builder";
    settings.beginGroup("datagram");
    DatagramBuilder* datagramBuilder = new DatagramBuilder(settings);
    settings.endGroup();

    qDebug()<<"Starting server";
    settings.beginGroup("server");
    Listener* listener = new Listener(settings);
    listener->setRegistry(registry);
    listener->setDatagramBuilder(datagramBuilder);
    listener->start();
    settings.endGroup();

    return app.exec();
}
