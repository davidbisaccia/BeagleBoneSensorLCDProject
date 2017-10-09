#ifndef WATERSENSORCONTROLLER_H
#define WATERSENSORCONTROLLER_H

#endif // WATERSENSORCONTROLLER_H

#include <QtCore>
#include <QThread>
#include <QTimer>

#include "include/Sensors/WaterSensor.h"

namespace BeagleBone
{
    namespace Controllers
    {
        class WaterSensorProcess : public QThread
        {

            Q_OBJECT

        public:
            WaterSensorProcess(QObject* parent = 0);
            virtual ~WaterSensorProcess();

            void Start();
            void Stop();

        signals:
            void WaterDetected(bool isWaterDetected);

        private slots:
            void WaterIntervalCheck();

        private:
            QTimer* _timer;
            bool _isWaterDetected;

            Sensors::WaterSensor* _waterSensor;
        };
    }
}
