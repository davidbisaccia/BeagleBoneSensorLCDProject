/*
 * GPIO.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * If you use this code in your work please cite:
 *   Derek Molloy, "Exploring BeagleBone: Tools and Techniques for Building
 *   with Embedded Linux", Wiley, 2014, ISBN:9781118935125.
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * I, David Bisaccia, have updated and reorganized the original code.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include "include/Core/GPIO.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
using namespace std;

namespace BeagleBone 
{

/**
 *
 * @param number The GPIO number for the BBB
 */
GPIO::GPIO(int number) 
{
	_number = number;
	_debounceTime = 0;
	_togglePeriod = 100;
	_toggleNumber = -1; //infinite number
	_callbackFunction = NULL;
	_threadRunning = false;

	ostringstream s;
	s << "gpio" << number;
	_name = string(s.str());
	_path = GPIO_PATH + _name + "/";
	ExportGPIO();
	
	// need to give Linux time to set up the sysfs structure
	usleep(250000); // 250ms delay
}

int GPIO::Write(string path, string filename, int value)
{
   stringstream s;
   s << value;
   
   return Write(path,filename,s.str());
}

int GPIO::Write(string path, string filename, string value)
{
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: write failed to open file ");
	   return -1;
   }
   
   fs << value;
   fs.close();
   
   return 0;
}

string GPIO::Read(string path, string filename)
{
   ifstream fs;
   fs.open((path + filename).c_str());
   
   if (!fs.is_open())
   {
	   perror("GPIO: read failed to open file ");
   }
	
   string input;
   getline(fs,input);
   fs.close();
   
   return input;
}

int GPIO::ExportGPIO()
{
   return Write(GPIO_PATH, "export", _number);
}

int GPIO::UnexportGPIO()
{
   return Write(GPIO_PATH, "unexport", _number);
}

int GPIO::SetDirection(GPIO_DIRECTION dir)
{
   switch(dir)
   {
	case INPUT: 
		return Write(_path, "direction", "in");
		break;
	case OUTPUT:
		return Write(_path, "direction", "out");
		break;
   }
   
   return -1;
}

int GPIO::SetValue(GPIO_VALUE value)
{
   switch(value)
   {
	case HIGH: 
		return Write(_path, "value", "1");
		break;
	case LOW: 
		return Write(_path, "value", "0");
		break;
   }
   
   return -1;
}

int GPIO::SetEdgeType(GPIO_EDGE value)
{
   switch(value)
   {
	case NONE: 
		return Write(_path, "edge", "none");
		break;
	case RISING: 
		return Write(_path, "edge", "rising");
		break;
	case FALLING: 
		return Write(_path, "edge", "falling");
		break;
	case BOTH: 
		return Write(_path, "edge", "both");
		break;
   }
   
	return -1;
}

int GPIO::SetActiveLow(bool isLow)
{
   if(isLow) 
	   return Write(_path, "active_low", "1");
    
	return Write(_path, "active_low", "0");
}

int GPIO::SetActiveHigh()
{
   return SetActiveLow(false);
}

GPIO_VALUE GPIO::GetValue()
{
	string input = Read(_path, "value");
	return (input == "0" ? LOW : HIGH);
}

GPIO_DIRECTION GPIO::GetDirection()
{
	string input = Read(_path, "direction");
	return (input == "in" ? INPUT : OUTPUT);
}

GPIO_EDGE GPIO::GetEdgeType()
{
	string input = Read(_path, "edge");
	if (input == "rising") return RISING;
	else if (input == "falling") return FALLING;
	else if (input == "both") return BOTH;
	
	return NONE;
}

int GPIO::StreamOpen()
{
	_stream.open((_path + "value").c_str());
	return 0;
}
int GPIO::StreamWrite(GPIO_VALUE value)
{
	_stream << value << std::flush;
	return 0;
}
int GPIO::StreamClose()
{
	_stream.close();
	return 0;
}

int GPIO::ToggleOutput()
{
	SetDirection(OUTPUT);
	SetValue((bool) GetValue() ? LOW : HIGH);
	
    return 0;
}

int GPIO::ToggleOutput(int time)
{ 
	return ToggleOutput(-1, time); 
}

int GPIO::ToggleOutput(int numberOfTimes, int time)
{
	SetDirection(OUTPUT);
	
	_toggleNumber = numberOfTimes;
	_togglePeriod = time;
	_threadRunning = true;
	
	if (pthread_create(&this->_thread, NULL, &ThreadedToggle, static_cast<void*>(this)))
	{
    	perror("GPIO: Failed to create the toggle thread");
    	_threadRunning = false;
    	return -1;
    }
	
    return 0;
}

// This thread function is a friend function of the class
void* ThreadedToggle(void *value)
{
	GPIO *gpio = static_cast<GPIO*>(value);
	
	bool isHigh = (bool) gpio->GetValue(); //find current value
	
	while(gpio->_threadRunning)
	{
		gpio->SetValue(isHigh ? HIGH : LOW);
		
		usleep(gpio->_togglePeriod * 500);
		isHigh = !isHigh;
		
		if(gpio->_toggleNumber > 0) 
			gpio->_toggleNumber--;
			
		if(gpio->_toggleNumber == 0) 
			gpio->_threadRunning = false;
	}
	
	return 0;
}

// Blocking Poll - based on the epoll socket code in the epoll man page
int GPIO::WaitForEdge()
{
	SetDirection(INPUT); // must be an input pin to poll its value
	int fd, i, epollfd, count = 0;
	struct epoll_event ev;
	
	epollfd = epoll_create(1);
    if (epollfd == -1) 
	{
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }
	
    if ((fd = open((this->_path + "value").c_str(), O_RDONLY | O_NONBLOCK)) == -1) 
	{
       perror("GPIO: Failed to open file");
       return -1;
    }

    //ev.events = read operation | edge triggered | urgent data
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;  // attach the file file descriptor

    //Register the file descriptor on the epoll instance, see: man epoll_ctl
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) 
	{
       perror("GPIO: Failed to add control interface");
       return -1;
    }
	
	while(count <= 1) // ignore the first trigger
	{  
		i = epoll_wait(epollfd, &ev, 1, -1);
		if (i==-1)
		{
			perror("GPIO: Poll Wait fail");
			count = 5; // terminate loop, error code
		}
		else 
		{
			count++; // count the triggers up
		}
	}
	
    close(fd);
	
	//5 is our error code
    return (count == 5 ? -1 : 0);
}

// This thread function is a friend function of the class
void* ThreadedPoll(void *value)
{
	GPIO *gpio = static_cast<GPIO*>(value);
	
	while(gpio->_threadRunning)
	{
		gpio->_callbackFunction(gpio->WaitForEdge());
		usleep(gpio->_debounceTime * 1000);
	}
	
	return 0;
}

int GPIO::WaitForEdge(CallbackType callback)
{
	_threadRunning = true;
	_callbackFunction = callback;
    // create the thread, pass the reference, address of the function and data
	if (pthread_create(&this->_thread, NULL, &ThreadedPoll, static_cast<void*>(this)))
	{
    	perror("GPIO: Failed to create the poll thread");
    	_threadRunning = false;
    	return -1;
    }
	
    return 0;
}

GPIO::~GPIO() 
{
	UnexportGPIO();
}

} /* namespace exploringBB */
