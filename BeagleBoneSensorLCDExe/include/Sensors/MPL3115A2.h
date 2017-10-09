#pragma once

#include "include/Core/I2CDevice.h"
#include "include/Sensors/MPL3115A2SensorData.h"

using namespace BeagleBone;

//NOTE: this is not tested and unused.

namespace BeagleBone
{
	namespace Sensors
	{
	
		class MPL3115A2 : protected I2CDevice
		{
		public:
			MPL3115A2(unsigned int bus, unsigned int deviceAddress = 0xC0);
			virtual ~MPL3115A2();
		
			MPL3115A2SensorData* ReadSensorData();
			
			
		private :
			bool OkToRead();
			bool ResetSensorToReadNextDataset();
			
			unsigned int _i2cBus;
			unsigned int _i2cDevice;
		};
	}
}
