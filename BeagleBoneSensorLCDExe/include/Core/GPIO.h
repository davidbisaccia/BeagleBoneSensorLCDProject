/*
 * GPIO.h  Created on: 29 Apr 2014
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
 * I, David Bisaccia, have updated and reorganized the original code.  Mostly just formatting changes
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef GPIO_H_
#define GPIO_H_

#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

namespace BeagleBone 
{

typedef int (*CallbackType)(int);
enum GPIO_DIRECTION{ INPUT, OUTPUT };
enum GPIO_VALUE{ LOW = 0, HIGH = 1 };
enum GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO 
{
	
private:
	int _number;
	int _debounceTime;
	std::string _name;
	std::string _path;

public:
	GPIO(int number); //constructor will export the pin
	virtual int GetNumber() { return _number; }

	// General Input and Output Settings
	virtual int SetDirection(GPIO_DIRECTION);
	virtual GPIO_DIRECTION GetDirection();
	virtual int SetValue(GPIO_VALUE);
	virtual int ToggleOutput();
	virtual GPIO_VALUE GetValue();
	virtual int SetActiveLow(bool isLow=true);  //low=1, high=0
	virtual int SetActiveHigh(); //default
	//software debounce input (ms) - default 0
	virtual void SetDebounceTime(int time) { _debounceTime = time; }

	// Advanced OUTPUT: Faster write by keeping the stream alive (~20X)
	virtual int StreamOpen();
	virtual int StreamWrite(GPIO_VALUE);
	virtual int StreamClose();

	virtual int ToggleOutput(int time); //threaded invert output every X ms.
	virtual int ToggleOutput(int numberOfTimes, int time);
	virtual void ChangeToggleTime(int time) { _togglePeriod = time; }
	virtual void ToggleCancel() { _threadRunning = false; }

	// Advanced INPUT: Detect input edges; threaded and non-threaded
	virtual int SetEdgeType(GPIO_EDGE);
	virtual GPIO_EDGE GetEdgeType();
	virtual int WaitForEdge(); // waits until button is pressed
	virtual int WaitForEdge(CallbackType callback); // threaded with callback
	virtual void WaitForEdgeCancel() { _threadRunning = false; }

	virtual ~GPIO();  //destructor will unexport the pin

private:
	int Write(string path, string filename, string value);
	int Write(string path, string filename, int value);
	string Read(string path, string filename);
	
	int ExportGPIO();
	int UnexportGPIO();
	
	ofstream _stream;
	pthread_t _thread;
	CallbackType _callbackFunction;
	bool _threadRunning;
	int _togglePeriod;  //default 100ms
	int _toggleNumber;  //default -1 (infinite)
	
	friend void* ThreadedPoll(void *value);
	friend void* ThreadedToggle(void *value);
};

void* ThreadedPoll(void *value);
void* ThreadedToggle(void *value);

} /* namespace exploringBB */

#endif /* GPIO_H_ */
