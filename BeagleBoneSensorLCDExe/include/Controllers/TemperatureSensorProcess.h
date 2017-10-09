#ifndef TEMPERATURESENSORPROCESS_H
#define TEMPERATURESENSORPROCESS_H

#endif // TEMPERATURESENSORPROCESS_H

#include <QtCore>
#include <QThread>
#include <QTimer>
#include "include/Sensors/TMP36.h"
//#include "include/Sensors/MPL3115A2.h"

namespace BeagleBone
{
    namespace Controllers
    {
        class TemperatureSensorProcess : public QThread
        {
            Q_OBJECT

        public:
            TemperatureSensorProcess(QObject* parent = 0);
            virtual ~TemperatureSensorProcess();

            void Start();
            void Stop();

        signals:
            void SendTemperatureAndPressure(const int& temperature, const int& pressure);
            void SendTemperature(const int& temperature);

        private slots:
            void GetUpdatedSensorData();

        private:
            QTimer* _timer;
            Sensors::TMP36* _sensor;
            //sensor we are no longer using
            //Sensors::MPL3115A2* _sensor;

        };
    }
}
