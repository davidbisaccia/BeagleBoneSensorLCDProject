#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#endif // ANALOGINPUT_H

#include <string>

#define ANALOG_PATH_PREFIX "/sys/bus/iio/devices/iio:device0/in_voltage"
#define ANALOG_PATH_SUFFIX "_raw"

namespace BeagleBone
{

    class AnalogInput
    {
    public:
        AnalogInput(int analogPin);
        virtual ~AnalogInput();

        //returns a value between 0 to 1.8 volts
        float GetValue() const;
    private:
        std::string _pinPath;
    };

}
