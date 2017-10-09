#pragma once

#include "include/Core/GPIO.h"

namespace BeagleBone
{
	
	namespace Sensors
	{
		
        class WaterSensor : protected GPIO
		{
		public:
            WaterSensor(int pinNumber);
            virtual ~WaterSensor();
			
			bool DetectAnyWater();
			
		private:
			int _pinNumber;
		};
		
	}
}
