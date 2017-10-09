#include "include/Sensors/TMP36.h"

namespace BeagleBone
{

    namespace Sensors
    {

        TMP36::TMP36(int pinNumber)
            : AnalogInput(pinNumber)
        {
        }

        TMP36::~TMP36()
        {
        }

        float TMP36::GetTemperatureCelsius() const
        {
            float voltage = GetValue();
            //convert to c
            return 100 * voltage - 50;
        }

        float TMP36::GetTemperatureFahrenheit() const
        {
            float voltage = GetValue();
            //convert to f
            return 180 * voltage - 58;
        }

    }
}
