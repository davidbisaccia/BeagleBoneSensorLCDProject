#include "include/Sensors/MPL3115A2.h"
#include <exception>
#include <cstdio>

//NOTE: this is not tested and unused.

namespace BeagleBone
{
	namespace Sensors
	{
		MPL3115A2::MPL3115A2(unsigned int bus, unsigned int deviceAddress)
			: I2CDevice(bus, deviceAddress), _i2cBus(bus), _i2cDevice(deviceAddress)
		{
			if (!Open())
				throw std::exception();//"Failed to initialize the MPL3115A2 sensor");
			
			//init the sensor 
			//set to alitimeter mode
			WriteRegister(0x26, 0xB8);
			
			//configures the flags for the sensors
			WriteRegister(0x13, 0x07);
		}
		
		MPL3115A2::~MPL3115A2()
		{
		}
		
		MPL3115A2SensorData* MPL3115A2::ReadSensorData()
		{
			//make sure we are good and set to read
			if (!OkToRead() || ResetSensorToReadNextDataset())
			{
				return NULL;
			}
				
			//Pressure
			unsigned char highPressure = ReadRegister(0x01);
			unsigned char midPressure = ReadRegister(0x02);
			unsigned char lowPressure = ReadRegister(0x03);
			int pressure = (int)lowPressure;
			pressure |= (int)midPressure << 2;
			pressure |= (int)highPressure << 6;
			
			//Temperature
			unsigned char highTemp = ReadRegister(0x04);
			unsigned char lowTemp = ReadRegister(0x05);
			int temp = (int)lowTemp;
			temp |= (int)highTemp << 2;
			
			MPL3115A2SensorData* retVal = new MPL3115A2SensorData();
			retVal->pressure = pressure;
			retVal->temperature = temp;
			return retVal;
		}
		
		bool MPL3115A2::OkToRead()
		{
			unsigned char canRead = ReadRegister(0x00);
			
			if (canRead & 0x08)
				return true;
			
			return false;
		}
		
		bool MPL3115A2::ResetSensorToReadNextDataset()
		{
			if (!WriteRegister(0x26, 0xB8))
				return false;
			
			if (!WriteRegister(0x26, 0xB9))
				return false;
			
			return true;
		}
	}
}
