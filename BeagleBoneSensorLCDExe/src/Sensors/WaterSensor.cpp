#include "include/Sensors/WaterSensor.h"

namespace BeagleBone
{
	
	namespace Sensors
	{
        WaterSensor::WaterSensor(int pinNumber)
			: GPIO(pinNumber), _pinNumber(pinNumber)
		{
			SetDirection(INPUT);
		}
		
        WaterSensor::~WaterSensor()
		{
		}
			
        bool WaterSensor::DetectAnyWater()
		{
			//it is high normally, but low when water hits the sensor
			return GetValue() == LOW;
		}
	}
}
