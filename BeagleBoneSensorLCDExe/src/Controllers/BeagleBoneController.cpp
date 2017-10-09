#include "include/Controllers/BeagleBoneController.h"

namespace BeagleBone
{
    namespace Controllers
    {
        BeagleBoneController::BeagleBoneController(QObject* parent)
            : QThread(parent), _stop(true), _temperatureThreshold(45), _temperatureTooLow(false),
              _waterDetected(false)
        {
            //create all the threads
            _lcdProcess = new LCDProcess(this);
            _buzzerProcess = new BuzzerProcess(this);
            _waterSensorProcess = new WaterSensorProcess(this);
            _temperatureProcess = new TemperatureSensorProcess(this);

            //hook up all the sensor threads with signals and slots
            connect( _temperatureProcess, SIGNAL(SendTemperature(int)), this, SLOT(ReceiveTemperatureData(int)) );
            connect( _waterSensorProcess, SIGNAL(WaterDetected(bool)), this, SLOT(ReceiveWaterDetectedStatus(bool)) );

            //hook up the sound lcd signals from this controller
            connect( this, SIGNAL(SendBuzzerUpdate(bool)), _buzzerProcess, SLOT(Beep(bool)) );
            connect( this, SIGNAL(SendPressureData(int)), _lcdProcess, SLOT(ReceivePressure(int)) );
            connect( this, SIGNAL(SendTemperatureData(int)), _lcdProcess, SLOT(ReceiveTemperature(int)) );
            connect( this, SIGNAL(SendWaterDetectedStatus(bool)), _lcdProcess, SLOT(WaterDetectedStatus(bool)) );
            connect( this, SIGNAL(SendTemperatureData(int)), _communicationProcess, SLOT(ReceiveTemperatureData(int)) );
            connect( this, SIGNAL(SendWaterDetectedStatus(bool)), _communicationProcess, SLOT(ReceiveWaterDetectedStatus(bool)) );

            //for commnication status messages
            connect(_communicationProcess, SIGNAL(SendStatus(QString)), this, SLOT(ShowStatusMessage(QString)) );

            //When we make a service that launches this, it takes a little bit
            //of time for the sensors to get valid data, so in order to avoid getting
            //any unwanted beeping we sleep 1 minute
            //other ways to consider handling this is to figure out a way to know when things have
            //been properly initialized, and then start all the process items.
            //tbh this is a little lazy, but it works for a simple project.  Would not recommend
            //doing this in professional development.
            sleep(60);

            //launch all the threads, do not need to start the Buzzer
            _lcdProcess->Start();
            _waterSensorProcess->Start();
            _temperatureProcess->Start();
            _communicationProcess->Start();
        }

        BeagleBoneController::~BeagleBoneController()
        {
            //kill all the threads
            Stop();
        }


        void BeagleBoneController::Start()
        {
            _lcdProcess->ShowMessage("Re-Starting...");
            _stop = false;

            //start threads
            _lcdProcess->Start();
            _waterSensorProcess->Start();
            _temperatureProcess->Start();
            _communicationProcess->Start();
        }

        void BeagleBoneController::Stop()
        {
            _lcdProcess->ShowMessage("Stoping");
            _stop = true;

            //stop threads
            _buzzerProcess->Stop();
            _lcdProcess->Stop();
            _waterSensorProcess->Stop();
            _temperatureProcess->Stop();
            _communicationProcess->Stop();
        }

        //not using this one right now
        //receive the data from the temperature and pressure sensor
        void BeagleBoneController::ReceivePressureAndTemperatureData(const int& temperature, const int& pressure)
        {
            //send the update to everyone listening, aka the LCD
            emit SendTemperatureData(temperature);
            emit SendPressureData(pressure);
        }

        void BeagleBoneController::ReceiveTemperatureData(const int& temperature)
        {
            //send the update to everyone listening, aka the LCD
            _lcdProcess->ShowMessage("TempDataRcv");
            emit SendTemperatureData(temperature);

            if(temperature < _temperatureThreshold)
            {
                _lcdProcess->ShowMessage("Temperature too low.");
                _temperatureTooLow = true;
            }
            else
            {
                _temperatureTooLow = false;
            }

            //update the buzzer to start or stop making noise
            UpdateBuzzerStatus();
        }

        //receieve a water status change
        void BeagleBoneController::ReceiveWaterDetectedStatus(bool isWater)
        {
            _lcdProcess->ShowMessage("H2ODataRcv");
            //update the LCD
            emit SendWaterDetectedStatus(isWater);

            _waterDetected = isWater;
            //update the buzzer to start or stop making noise
            UpdateBuzzerStatus();
        }

        void BeagleBoneController::UpdateBuzzerStatus()
        {
            bool sound = _waterDetected || _temperatureTooLow;
            SendBuzzerUpdate(sound);
        }

        void BeagleBoneController::ShowStatusMessage(const QString& status)
        {
            QTextStream(stdout) << status;
        }
    }
}
