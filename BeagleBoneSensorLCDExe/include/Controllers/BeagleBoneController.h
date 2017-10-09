#pragma once

#include <QtCore>
#include <QThread>

#include "include/Controllers/TemperatureSensorProcess.h"
#include "include/Controllers/WaterSensorProcess.h"
#include "include/Controllers/BuzzerProcess.h"
#include "include/Controllers/LCDProcess.h"
#include "include/Controllers/CommunicationProcess.h"

namespace BeagleBone
{
    namespace Controllers
    {
        class BeagleBoneController : public QThread
        {
            Q_OBJECT

        public:
            explicit BeagleBoneController(QObject* parent = 0);
            virtual ~BeagleBoneController();

            void Start();
            void Stop();

        signals:
            //these mostly go to the "UI", the LCD, and the buzzer in this case
            void SendTemperatureData(const int& temperature);
            void SendPressureData(const int& pressure);
            void SendWaterDetectedStatus(bool isWater);

            void SendBuzzerUpdate(bool makeNoise);

        public slots:
            //this is where we will be receiving data from the sensors
            void ReceiveTemperatureData(const int& temperature);
            void ReceivePressureAndTemperatureData(const int& temperature, const int& pressure);
            void ReceiveWaterDetectedStatus(bool isWater);
            void ShowStatusMessage(const QString& status);

        private:
            bool _stop;
            int _temperatureThreshold;
            bool _temperatureTooLow;
            bool _waterDetected;

            void UpdateBuzzerStatus();

            BuzzerProcess* _buzzerProcess;
            WaterSensorProcess* _waterSensorProcess;
            TemperatureSensorProcess* _temperatureProcess;
            LCDProcess* _lcdProcess;
            CommunicationProcess *_communicationProcess;
        };
    }
}
