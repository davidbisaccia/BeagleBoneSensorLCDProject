#ifndef UART_H_
#define UART_H_

#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define UARTPATH "/dev/ttyO"

namespace BeagleBone
{
	class UART
	{
	
	public:
        UART(unsigned int number); //TODO: add baud rate? add bit as well?
		virtual ~UART();
		
        bool Write(unsigned char value);
        //std::string Read(int size);
        void Close();
	
	private:
		std::string _name;
		std::string _path;
		int _file;
		bool _open;
	};
}

#endif
