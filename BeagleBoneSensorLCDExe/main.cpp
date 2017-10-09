#include <QtCore>
#include <QCoreApplication>
#include <QString>

#include "include/Controllers/TestController.h"
#include "include/Controllers/BeagleBoneController.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //run in normal mode
    if(argc == 1)
    {
        QTextStream(stdout) << "Starting, process.  Press any key and enter to stop the program.";

        BeagleBone::Controllers::BeagleBoneController *controller = new BeagleBone::Controllers::BeagleBoneController(&a);
        //controller->Start();

        a.exec();

        QTextStream s(stdin);
        QString val = s.readLine();

        controller->Stop();

        QTextStream(stdout) << "Stopping.....";
        a.quit();
    }
    else if(argc == 2) //test mode
    {
        QString testTypeStr = argv[1];

        BeagleBone::Controllers::TestType testType = BeagleBone::Controllers::TestType::Buzzer;
        if(testTypeStr == "buzzer")
        {
            testType = BeagleBone::Controllers::TestType::Buzzer;
        }
        else if(testTypeStr == "lcd")
        {
            testType = BeagleBone::Controllers::TestType::LCD;
        }
        else if(testTypeStr == "water")
        {
            testType = BeagleBone::Controllers::TestType::WaterSensor;
        }
        else if(testTypeStr == "temperature")
        {
            testType = BeagleBone::Controllers::TestType::TemperatureSensor;
        }
        else
        {
            QTextStream(stdout) << "Invalid test parameter. Please use buzzer, lcd, temperature, or water.";
            return 0; //return early do not do anything
        }

        BeagleBone::Controllers::TestController* testController = new BeagleBone::Controllers::TestController(testType, &a);
        testController->run();
    }

    return a.exec();
}
