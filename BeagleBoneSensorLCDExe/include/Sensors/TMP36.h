#ifndef TMP36_H
#define TMP36_H

#endif // TMP36_H

#include "include/Core/AnalogInput.h"

namespace BeagleBone
{
    namespace Sensors
    {

        class TMP36 : public AnalogInput
        {
        public:
            TMP36(int pinNumber);
            virtual ~TMP36();

            float GetTemperatureCelsius() const;
            float GetTemperatureFahrenheit() const;
        };

    }
}
