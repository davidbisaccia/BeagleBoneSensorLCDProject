#include "include/Sound/Buzzer.h"

namespace BeagleBone
{
	namespace Sound
	{
		Buzzer::Buzzer(int pinNumber)
			: GPIO(pinNumber), _on(false)
		{
			SetDirection(OUTPUT);
            SetSpeaker(false);
		}
		
		Buzzer::~Buzzer()
		{
		}
		
		void Buzzer::SetSpeaker(bool on)
		{
			_on = on;
			SetValue(on ? BeagleBone::GPIO_VALUE::HIGH : BeagleBone::GPIO_VALUE::LOW);
		}
		
		bool Buzzer::IsSpeakerOn() const
		{
			return _on;
		}
		
	}
}
