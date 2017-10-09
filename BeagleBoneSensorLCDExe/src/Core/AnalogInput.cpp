#include "include/Core/AnalogInput.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

namespace BeagleBone
{
    AnalogInput::AnalogInput(int analogPin)
    {
        std::ostringstream s;
        s << ANALOG_PATH_PREFIX << analogPin << ANALOG_PATH_SUFFIX;
        _pinPath = std::string(s.str());
    }

    AnalogInput::~AnalogInput()
    {
    }

    //returns a value between 0 to 1.8 volts
    float AnalogInput::GetValue() const
    {
        float value = 0;

        std::ifstream fs;
        fs.open(_pinPath.c_str());

        if (!fs.is_open())
        {
            perror("Analog Input: read failed to open file ");
            perror(_pinPath.c_str());
        }

        std::string input;
        std::getline(fs,input);
        fs.close();

        try
        {
            value = static_cast<float>(stoi(input));
            value = 1.8 * value / 4095;
        }
        catch(std::exception ex)
        {
            value = 0;
            perror("Analog Input: read failed to convert value to float");
        }

        return value;
    }

}
