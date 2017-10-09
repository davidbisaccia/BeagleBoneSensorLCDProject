#include "include/Controllers/WaterSensorProcess.h"
#include "include/GPIOInfo.h"

namespace BeagleBone
{
    namespace Controllers
    {
        WaterSensorProcess::WaterSensorProcess(QObject* parent)
            :QThread(parent), _isWaterDetected(false)
        {
            _timer = new QTimer(this);
            _waterSensor = new Sensors::WaterSensor(BeagleBone::GPIOInfo::WaterSensorGPIO);

            //connect timer to checking function
            connect( _timer, SIGNAL(timeout()), this, SLOT(WaterIntervalCheck()) );
        }

        WaterSensorProcess::~WaterSensorProcess()
        {
            Stop();
        }

        void WaterSensorProcess::Start()
        {
            //QTextStream(stdout) << "\nH2O Sensor starting...";

            //initial state check
            WaterIntervalCheck();

            _timer->start(12000);
        }

        void WaterSensorProcess::Stop()
        {
            //QTextStream(stdout) << "\nH2O Sensor stopping.";
            _timer->stop();
        }

        void WaterSensorProcess::WaterIntervalCheck()
        {
            //QTextStream(stdout) << "\nH2O Sensor checking.";
            bool isWaterDetected = _waterSensor->DetectAnyWater();

            //if the status has changed since our last check, then send an update
            if(isWaterDetected != _isWaterDetected)
            {
                _isWaterDetected = isWaterDetected;
                emit WaterDetected(_isWaterDetected);
            }
        }
    }
}
