#include "include/Controllers/TemperatureSensorProcess.h"
#include "include/GPIOInfo.h"

namespace BeagleBone
{
    namespace Controllers
    {
        TemperatureSensorProcess::TemperatureSensorProcess(QObject* parent)
            : QThread(parent)
        {
            _timer = new QTimer(this);
            //_sensor = new Sensors::MPL3115A2(BeagleBone::I2cInfo::TemperatureBus);

            _sensor = new Sensors::TMP36(BeagleBone::GPIOInfo::TemperatureAnalogPin);

            connect( _timer, SIGNAL(timeout()), this, SLOT(GetUpdatedSensorData()) );
        }

        TemperatureSensorProcess::~TemperatureSensorProcess()
        {
            Stop();
        }

        void TemperatureSensorProcess::Start()
        {
            //QTextStream(stdout) << "\nTemp Sensor starting...";

            //do the initial temperature check
            GetUpdatedSensorData();
            _timer->start(60000);
        }

        void TemperatureSensorProcess::Stop()
        {
            //QTextStream(stdout) << "\nTemp Sensor stopping...";
            _timer->stop();
        }

        void TemperatureSensorProcess::GetUpdatedSensorData()
        {
            //QTextStream(stdout) << "\nTemp Sensor getting update";

            //This is when we were using this, but I switched to the TMP36 for my needs
            //I left it here in case anyone wanted to eventually use it
            //Sensors::MPL3115A2SensorData* data = _sensor->ReadSensorData();
            //emit SendTemperatureAndPressure(data->temperature, data->pressure);
            float temp = _sensor->GetTemperatureFahrenheit();
            emit SendTemperature((int)temp);
        }
    }
}
