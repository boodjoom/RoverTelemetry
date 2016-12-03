#include "imucomm.h"

#ifndef WIN32
#   define WIN32 1
#endif
#ifdef USE_RTIMULib
#include "RTIMULib.h"
#endif
#include <QDebug>
#include <QScopedPointer>
#include <QThread>

ImuComm::ImuComm(TeleData *teleData, QObject *parent) : QObject(parent)
  , _teleData(teleData)
  , _mustExit(false)
  , _settingsDir("/etc")
{
    Q_ASSERT(_teleData);
}

void ImuComm::run()
{
#ifdef USE_RTIMULib
    int sampleCount = 0;
    int sampleRate = 0;
    uint64_t rateTimer;
    uint64_t displayTimer;
    uint64_t now;
    bool ismoving = false;
    RTVector3 residuals;
    RTFLOAT heading;
    float heading_avg = 0.0f;
    RTIMU_DATA imuData;
    QScopedPointer<RTIMUSettings> settings(new RTIMUSettings(_settingsDir.toLocal8Bit().constData(),"RTIMULib"));
    QScopedPointer<RTIMU> imu(RTIMU::createIMU(settings.data()));
    if ((imu.isNull()) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        qCritical("ImuComm: No IMU found");
        return;
    }
    QScopedPointer<RTMotion> motion(new RTMotion(settings.data()));
    motion->motionInit();

    //  set up IMU
    imu->IMUInit();
    //  this is a convenient place to change fusion parameters
    imu->setGyroRunTimeCalibrationEnable(false);             // turn off gyro bias calibration at startup, allow system to equiblirate first
    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);
    imu->setDebugEnable(false); // turn on if you want to see Fusion information
    // dry run of the system
    int i=0;
    while (i < 80) {
        if  (imu->IMURead()) { i++; }
    }
    imu->setGyroRunTimeCalibrationEnable(true);

    //  set up for rate timer
    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
#endif

    while(!_mustExit)
    {
        //  poll at the rate recommended by the IMU
#ifdef USE_RTIMULib
        QThread::msleep(imu->IMUGetPollInterval());
        while (imu->IMURead())
        {
            imuData = imu->getIMUData();
            // Motion detection
            ismoving=motion->detectMotion(imuData.accel, imuData.gyro);
            // Residuals of Acceleration minus Gravity
            residuals = imu->getAccelResiduals();
            // Tilt Compensated Heading
            heading     = imuData.fusionPose.toHeading(imuData.compass, settings->m_compassAdjDeclination);
            heading_avg = motion->updateAverageHeading(heading); // smooth it out to approx. 10Hz update rate
            // Attempt velocity and position estimation
            motion->updateVelocityPosition(residuals, imuData.fusionQPose, 9.81f, imuData.timestamp, ismoving);
            // checking rotations: motion->updateVelocityPosition(imuData.accel, imuData.fusionQPose, 9.81f, imuData.timestamp, ismoving);
            MOTION_DATA motionData = motion->getMotionData();

            sampleCount++;

            now = RTMath::currentUSecsSinceEpoch();

            //  display 50 times per second
//            if ((now - displayTimer) > 20000)
//            {
                //Publish data
                _teleData->imuData.accel    =   imuData.accel;
                _teleData->imuData.gyro     =   imuData.gyro;
                _teleData->imuData.fusionPose   = imuData.fusionPose;
                _teleData->imuData.compas = imuData.compass;
                _teleData->imuData.temperature = imuData.IMUtemperature;
                _teleData->imuData.motionData = motionData;
                _teleData->imuData.motionData.motion = ismoving;
                _teleData->imuData.heading = heading_avg;
                _teleData->imuDataValid = true;
//                emit publish();
//                displayTimer = now;
//            }

            //  update rate every second

            if ((now - rateTimer) > 1000000) {
                sampleRate = sampleCount;
                sampleCount = 0;
                rateTimer = now;
            }

        }
#endif

    }
    emit finished();
}
