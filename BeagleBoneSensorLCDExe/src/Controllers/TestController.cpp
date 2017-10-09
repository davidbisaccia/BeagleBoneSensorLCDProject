#include "include/Controllers/TestController.h"

#include"include/Sound/Buzzer.h"
#include "include/Display/NewHavenLCDDisplay.h"
#include "include/Sensors/WaterSensor.h"
#include "include/Sensors/MPL3115A2.h"
#include "include/Sensors/TMP36.h"

#include "include/I2CInfo.h"
#include "include/GPIOInfo.h"
#include "include/UARTInfo.h"


namespace BeagleBone
{
    namespace Controllers
    {
        TestController::TestController(TestType type, QCoreApplication* parent)
            : _type(type), QThread(parent)
        {
            connect(this, SIGNAL(finished()), parent, SLOT(quit()) );
        }

        TestController::~TestController()
        {
            this->terminate();
        }

        void TestController::run()
        {
            QTextStream(stdout) << "\nBeginning test.....";

            switch(_type)
            {
            case Buzzer:
                BuzzerTest();
                break;
            case LCD:
                LCDTest();
                break;
            case WaterSensor:
                WaterSensorTest();
                break;
            case TemperatureSensor:
                TemperatureSensorTest();
                break;
            default:
                QTextStream(stdout) << "\nUnknown test....";
                break;
            }


            QTextStream(stdout) << "\nTest complete.";

            emit TestComplete();
        }

        //beeps three times one second on, one off
        void TestController::BuzzerTest()
        {
            try
            {
                QTextStream(stdout) << "\nBuzzer Test";

                Sound::Buzzer buzzer(BeagleBone::GPIOInfo::BuzzerGPIO);

                QTextStream(stdout) << "\nBuzzer Object Created";
                buzzer.SetSpeaker(false);
                QTextStream(stdout) << "\nBuzzer Off";
                sleep(1);
                buzzer.SetSpeaker(true);
                QTextStream(stdout) << "\nBuzzer On";
                sleep(1);
                buzzer.SetSpeaker(false);
                QTextStream(stdout) << "\nBuzzer Off";
                sleep(1);
                buzzer.SetSpeaker(true);
                QTextStream(stdout) << "\nBuzzer On";
                sleep(1);
                buzzer.SetSpeaker(false);
                QTextStream(stdout) << "\nBuzzer Off";
                sleep(1);
                buzzer.SetSpeaker(true);
                QTextStream(stdout) << "\nBuzzer On";
                sleep(1);
                buzzer.SetSpeaker(false);
                QTextStream(stdout) << "\nBuzzer Off";
            }
            catch(QException ex)
            {
                QTextStream(stdout) << "\nTest failed: " << ex.what();
                return;
            }

            QTextStream(stdout) << "\nTest successful";
        }

        void TestController::LCDTest()
        {
            try
            {
                Display::NewHavenLCDDisplay lcd(BeagleBone::UARTInfo::LCDUART);
                lcd.ClearDisplay();
                lcd.RunDisplayTest();
            }
            catch(QException ex)
            {
                QTextStream(stdout) << "\nTest failed: " << ex.what();
                return;
            }


            QTextStream(stdout) << "\nTest successful";
        }

        void TestController::WaterSensorTest()
        {
            try
            {
                Sensors::WaterSensor water(BeagleBone::GPIOInfo::WaterSensorGPIO);
                QTextStream(stdout) << "\nWater Status: " << (water.DetectAnyWater() ? "Detected" : "None");
            }
            catch(QException ex)
            {
                QTextStream(stdout) << "\nTest failed: " << ex.what();
                return;
            }


            QTextStream(stdout) << "\nTest successful";
        }

        void TestController::TemperatureSensorTest()
        {
            try
            {
                Sensors::TMP36 temp(BeagleBone::GPIOInfo::TemperatureAnalogPin);
                QTextStream(stdout) << "\nTemperature F: " << temp.GetTemperatureFahrenheit();
                QTextStream(stdout) << "\nTemperature C: " << temp.GetTemperatureCelsius();
            }
            catch(QException ex)
            {
                QTextStream(stdout) << "\nTest failed: " << ex.what();
                return;
            }

            QTextStream(stdout) << "\nTest successful";
        }
    }
}
