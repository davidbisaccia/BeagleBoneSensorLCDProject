#pragma once

#include "include/Core/GPIO.h"

namespace BeagleBone
{
	namespace Sound
	{
		class Buzzer : protected GPIO
		{
		public:
			Buzzer(int pinNumber);
			virtual ~Buzzer();
			
			void SetSpeaker(bool on);
			bool IsSpeakerOn() const;
			
		private:
			bool _on;
		};
	}
}
