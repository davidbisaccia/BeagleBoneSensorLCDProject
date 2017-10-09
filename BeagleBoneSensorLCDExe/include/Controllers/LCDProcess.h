#ifndef LCDPROCESS_H
#define LCDPROCESS_H

#endif // LCDPROCESS_H

#include <QtCore>
#include <QThread>
#include <QTimer>
#include "include/Display/NewHavenLCDDisplay.h"

namespace BeagleBone
{
    namespace Controllers
    {
        class LCDProcess : public QThread
        {
            Q_OBJECT

        public:
            LCDProcess(QObject* parent = 0);
            virtual ~LCDProcess();

            void Start();
            void Stop();

            void ShowMessage(QString msg);

        public slots:
            void ReceiveTemperature(const int& temperature);
            void ReceivePressure(const int& pressure);
            void WaterDetectedStatus(bool waterDetected);

        private slots:
            void UpdateDisplay();

        private:
            //QTimer* _timer;
            bool _hasUpdate;

            int _pressure;
            int _temperature;
            bool _water;

            Display::NewHavenLCDDisplay* _lcd;
        };
    }
}
