#include "include/Display/NewHavenLCDDisplay.h"

namespace BeagleBone
{
	namespace Display
	{
        NewHavenLCDDisplay::NewHavenLCDDisplay(unsigned int uart)
            : UART(uart), _uart(uart)
		{
            //initialize them all to spaces
            for (int idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
            {
                _firstLineConverted[idx] = START_INDEX;
                _secondLineConverted[idx] = START_INDEX;
                _thirdLineConverted[idx] = START_INDEX;
            }
		}
		
		NewHavenLCDDisplay::~NewHavenLCDDisplay()
		{
		}
		
		// We basically only return false if there were too many chars in a line or an 
		//error in communication to update the display. 
		//If a characterw as not supported, we will still take what we can and display it, 
		//unsupported characters will become spaces.
        bool NewHavenLCDDisplay::SetDisplayText(QString line, QString line2)
		{
			bool retVal = true;
			if (line.size() > 21)
            {
                line.truncate(MAX_CHARACER_ON_DISPLAY_LINE);
				retVal = false;
            }
            _firstLine = line;
			
			if (line2.size() > 21)
			{
                line2.truncate(MAX_CHARACER_ON_DISPLAY_LINE);
				retVal = false;
            }
            _secondLine = line2;

			
            for (int idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
			{
				_firstLineConverted[idx] = (idx >= _firstLine.size()) ? START_INDEX : TranslateCharacter(_firstLine[idx]);
				/* optional way using if else, if peeps don't like turnary operator
				if (idx >= _firstLine.size())
				{
					_firstLineConverted[idx] = START_INDEX;
				}
				else
				{
					_firstLineConverted[idx] = TranslateCharacter(_firstLine[idx]);
				}*/
			}
			
            for (int idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
			{
				_secondLineConverted[idx] = (idx >= _secondLine.size()) ? START_INDEX : TranslateCharacter(_secondLine[idx]);
				/* optional way using if else, if peeps don't like turnary operator
				if (idx >= _secondLine.size())
				{
					_secondLineConverted[idx] = START_INDEX;
				}
				else
				{
					_secondLineConverted[idx] = TranslateCharacter(_secondLine[idx]);
				}*/
			}
			
            retVal |= UpdateDisplay();
            return retVal;
		}

        bool NewHavenLCDDisplay::ShowMessage(QString msg)
        {
            bool retVal = true;
            if (msg.size() > 21)
            {
                msg.truncate(MAX_CHARACER_ON_DISPLAY_LINE);
                retVal = false;
            }

            _thirdLine = msg;

            for (int idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
            {
                _thirdLineConverted[idx] = (idx >= _thirdLine.size()) ? START_INDEX : TranslateCharacter(_thirdLine[idx]);
                /* optional way using if else, if peeps don't like turnary operator
                if (idx >= _firstLine.size())
                {
                    _firstLineConverted[idx] = START_INDEX;
                }
                else
                {
                    _firstLineConverted[idx] = TranslateCharacter(_firstLine[idx]);
                }*/
            }

            retVal |= UpdateDisplay();
            return retVal;
        }

        bool NewHavenLCDDisplay::UpdateDisplay()
        {
            bool retVal = true;
            retVal |= ClearDisplay();
            retVal |= WriteFirstLine();
            retVal |= WriteSecondLine();
            retVal |= WriteThirdLine();
            return retVal;
        }
		
		bool NewHavenLCDDisplay::ClearDisplay()
		{
			if (!Write(COMMAND_START))
				return false;
			
            usleep(100);

			if(!Write(0x51)) //clear screen command
				return false;
			
            usleep(100);

			return true;
		}
			
		bool NewHavenLCDDisplay::DisplayOn()
		{
			if (!Write(COMMAND_START))
				return false;
			
            usleep(100);

			if (!Write(0x41)) //display on command
				return false;

            usleep(100);
			
			return true;
		}
		
		bool NewHavenLCDDisplay::DisplayOff()
		{
			if (!Write(COMMAND_START))
				return false;

            usleep(100);

			if (!Write(0x42)) //display off command
				return false;
			
            usleep(100);

			return true;
		}
			
		bool NewHavenLCDDisplay::ShowI2CAddress() 
		{
			if (!Write(COMMAND_START))
				return false;

            usleep(100);
			
			if (!Write(0x72)) //display i2c address, use for debugging
				return false;

            usleep(100);
			
			return true;
		}
			
		void NewHavenLCDDisplay::RunDisplayTest()
		{
			ClearDisplay();
			
            //display all normall characters
			for (size_t idx = 0; idx < 95; idx++)
			{
				unsigned char test = START_INDEX + idx;
				Write(test);
                usleep(200000);
			}

            //part 2
            QString line1Test = "Here goes...";
            QString line2Test ="A simple test.";
            SetDisplayText(line1Test, line2Test);
		}
		
		bool NewHavenLCDDisplay::WriteFirstLine()
		{
			//reset cursor to first line, first position
			//0x46
			if (!Write(COMMAND_START))
				return false;
			
            usleep(100);

			if (!Write(0x46)) //reset cursor to first line, first position
				return false;

            usleep(100);
			
			//write the first line
			bool retVal = true;
			for (size_t idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
			{
				retVal |= Write(_firstLineConverted[idx]);
                usleep(100);
			}
			
			return retVal;
		}
		
		bool NewHavenLCDDisplay::WriteSecondLine()
		{
			//start cmd + command + parameter 
			//0xFE        0x45      0x40
			//reset cursor to second line, first position
			if (!Write(COMMAND_START))
				return false;
			
            usleep(100);

			if (!Write(0x45)) //set curosr position
				return false;
			
            usleep(100);

			if (!Write(0x40)) //second line, first column
				return false;

            usleep(100);
			
			//write the first line
			bool retVal = true;
			for (size_t idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
			{
                retVal |= Write(_secondLineConverted[idx]);
                usleep(100);
			}
			
			return retVal;
		}

        bool NewHavenLCDDisplay::WriteThirdLine()
        {
            //start cmd + command + parameter
            //0xFE        0x45      0x40
            //reset cursor to second line, first position
            if (!Write(COMMAND_START))
                return false;

            usleep(100);

            if (!Write(0x45)) //set curosr position
                return false;

            usleep(100);

            if (!Write(0x14)) //third line, first column
                return false;

            usleep(100);

            //write the first line
            bool retVal = true;
            for (size_t idx = 0; idx < MAX_CHARACER_ON_DISPLAY_LINE; idx++)
            {
                retVal |= Write(_thirdLineConverted[idx]);
                usleep(100);
            }

            return retVal;
        }
			
		//just a helper, if the value is outside the range, a space will be returned.
        unsigned char NewHavenLCDDisplay::TranslateCharacter(QChar a)
		{
            int val = a.toLatin1() - ' ';
			unsigned char retVal = START_INDEX;

            if (val < 95)
			{
				retVal += val;
			}
			
			return retVal;
		}
	}
}
