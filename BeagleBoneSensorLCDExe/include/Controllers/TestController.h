#pragma once

#include <QtCore>

namespace BeagleBone
{
    namespace Controllers
    {
        enum TestType
        {
            Buzzer = 0,
            LCD = 1,
            WaterSensor = 2,
            TemperatureSensor = 3
        };

        class TestController : public QThread
        {

            Q_OBJECT

        public:
            TestController(TestType type, QCoreApplication* parent = 0);
            ~TestController();

            virtual void run();

        signals:
            void TestComplete();

        private:
            void BuzzerTest();
            void LCDTest();
            void WaterSensorTest();
            void TemperatureSensorTest();

            TestType _type;
        };
    }
}
