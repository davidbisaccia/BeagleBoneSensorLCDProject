/*
 * I2C.h  Created on: 17 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef I2C_H_
#define I2C_H_

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"

namespace BeagleBone 
{

/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any type of I2C device and read or write to its registers
 */
class I2CDevice
{
private:
	unsigned int _bus;
	unsigned int _device;
	int _file;
	bool _open;
	
public:
	I2CDevice(unsigned int bus, unsigned int device);
	virtual bool Open();
	virtual bool Write(unsigned char value);
	virtual unsigned char ReadRegister(unsigned int registerAddress);
	virtual unsigned char* ReadRegisters(unsigned int number, unsigned int fromAddress = 0);
	virtual bool WriteRegister(unsigned int registerAddress, unsigned char value);
	virtual void DebugDumpRegisters(unsigned int number = 0xff);
	virtual void Close();
	virtual ~I2CDevice();
};

} /* namespace exploringBB */

#endif /* I2C_H_ */
