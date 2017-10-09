#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<sstream>

#include "include/Core/UART.h"

using namespace std;

namespace BeagleBone
{
    UART::UART(unsigned int number)
	{
       ostringstream s;
       s << number;
       _name = string(s.str());
		
       _path = UARTPATH + _name;
		
       if ((_file = open(_path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	   {
		  perror("UART: Failed to open the file.\n");
		  return;
	   }
	   
	   struct termios options;               //The termios structure is vital
       tcgetattr(_file, &options);            //Sets the parameters associated with file

	   // Set up the communications options:
	   //   9600 baud, 8-bit, enable receiver, no modem control lines
	   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	   options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
	   tcflush(_file, TCIFLUSH);             //discard file information not transmitted
	   tcsetattr(_file, TCSANOW, &options);  //changes occur immmediately
	   
       _open = true;
    }

	UART::~UART()
	{
		close(_file);
	}

    bool UART::Write(unsigned char value)
	{
		if(!_open)
			return false;
		
       return (write(_file, &value, sizeof(unsigned char)) >= 0);
	}

	//TODO: read data, I have no need for this so I did not test this
    /*
    std::string UART::Read(int size)
	{
		if(!_open)
            return "";
		
        int count = 0;
		unsigned char receive[size];      //declare a buffer for receiving data
		if ((count = read(_file, (void*)receive, size)) <0 )
		{
			perror("Failed to read from the input\n");
            return "";
		}
		
        if (count == 0)
		{
			printf("There was no data available to read!\n");
            return "";
		}
		
        //TODO: check this works whenever I use it
       std::string retVal(reinterpret_cast<char*>(receive), size);
	   return retVal;
    }*/

	void UART::Close()
	{
		if(_open)
			close(_file);
	}

}
