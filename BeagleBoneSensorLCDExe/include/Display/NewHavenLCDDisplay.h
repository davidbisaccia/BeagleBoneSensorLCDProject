//NHD - 0220D3Z - FL - GBW - V3
//Serial Liquid Crystal Display Module


#pragma once

#include "include/Core/UART.h"
#include <QString>
#include <unistd.h>

namespace BeagleBone
{
	namespace Display
	{
        class NewHavenLCDDisplay : protected UART
		{
		public:
            NewHavenLCDDisplay(unsigned int bus);
			virtual ~NewHavenLCDDisplay();
			
            bool SetDisplayText(QString line, QString line2);
			bool ClearDisplay();
			
			bool DisplayOn();
			bool DisplayOff();
			
			bool ShowI2CAddress();
			void RunDisplayTest();

            bool ShowMessage(QString msg);
			
		private:
            bool UpdateDisplay();
			bool WriteFirstLine();
			bool WriteSecondLine();
            bool WriteThirdLine();
			
			//just a helper
            static unsigned char TranslateCharacter(QChar a);
			
            unsigned int _uart;
            //unsigned int _i2cDevice;
			
            QString _firstLine;
            QString _secondLine;
            QString _thirdLine;
			
			//constants, made them static because no matter how many times we instantiate
			//this class we only need one copy of them in memory
			//aka, if we have multiple displays we are supporting
            const static unsigned char START_INDEX = 0x20; //first index is a space
            const static int MAX_CHARACER_ON_DISPLAY_LINE = 20;
			const static unsigned char COMMAND_START = 0xFE;
			
			unsigned char _firstLineConverted[20];
			unsigned char _secondLineConverted[20];
            unsigned char _thirdLineConverted[20];
		};
	}
}
