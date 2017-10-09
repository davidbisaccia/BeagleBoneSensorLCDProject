
#include "include/Controllers/LCDProcess.h"
#include "include/UARTInfo.h"

namespace BeagleBone
{
    namespace Controllers
    {

        LCDProcess::LCDProcess(QObject* parent)
            : QThread(parent), _hasUpdate(false), _water(false), _temperature(0),_pressure(0)
        {
            _lcd = new Display::NewHavenLCDDisplay(BeagleBone::UARTInfo::LCDUART);
            _lcd->SetDisplayText("Initializing, please", "wait for sensors.");
        }

        LCDProcess::~LCDProcess()
        {
            Stop();
        }

        void LCDProcess::Start()
        {
            //QTextStream(stdout) << "\nLCD starting...";
            UpdateDisplay();
        }

        void LCDProcess::Stop()
        {
        }

        void LCDProcess::ReceiveTemperature(const int& temperature)
        {
            //QTextStream(stdout) << "LCD Rcv temp update";

            if(temperature != _temperature)
            {
                _temperature = temperature;
                _hasUpdate = true;
                UpdateDisplay();
            }
        }

        //currently not using this
        void LCDProcess::ReceivePressure(const int& pressure)
        {
            //QTextStream(stdout) << "LCD Rcv pressure update";

            if(pressure != _pressure)
            {
                _pressure = pressure;
                _hasUpdate = true;
                UpdateDisplay();
            }
        }

        void LCDProcess::WaterDetectedStatus(bool waterDetected)
        {
            //QTextStream(stdout) << "LCD Rcv H2O update";

            if(waterDetected != _water)
            {
                _water = waterDetected;
                _hasUpdate = true;
                UpdateDisplay();
            }
        }

        void LCDProcess::UpdateDisplay()
        {
            if(_hasUpdate)
            {
                //NOTE Update for pressure if you want to add it, and have something capturing it
                QString line1 = QString("Temp %1").arg(_temperature);
                QString line2 = QString("Water %1").arg((_water ? "Yes" : "No"));

                QTextStream(stdout) << "\nLCD Current Display:";
                QTextStream(stdout) << "\n1: " << line1;
                QTextStream(stdout) << "\n2: " << line2;

                _lcd->SetDisplayText(line1, line2);

                _hasUpdate = false;
            }
        }

        void LCDProcess::ShowMessage(QString msg)
        {
            _lcd->ShowMessage(msg);
        }
    }
}
