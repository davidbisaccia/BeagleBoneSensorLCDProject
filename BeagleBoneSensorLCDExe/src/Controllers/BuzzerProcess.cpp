
#include "include/Controllers/BuzzerProcess.h"
#include "include/GPIOInfo.h"

namespace BeagleBone
{
    namespace Controllers
    {

        BuzzerProcess::BuzzerProcess(QObject* parent)
            : QThread(parent), _isBeepingNow(false)
        {
            _buzzer = new Sound::Buzzer(BeagleBone::GPIOInfo::BuzzerGPIO);
            _timer = new QTimer(this);

            connect( _timer, SIGNAL(timeout()), this, SLOT(UpdateBuzzer()) );
        }

        BuzzerProcess::~BuzzerProcess()
        {
            Stop();
        }

        void BuzzerProcess::Stop()
        {
            _timer->stop();
            _buzzer->SetSpeaker(false);
        }

        void BuzzerProcess::Beep(bool soundOn)
        {
            //if it is already on let it keep going
            if(soundOn)
            {
                if(!_isBeepingNow)
                {
                    _isBeepingNow = true;
                    _timer->start(500);
                }
            }
            else
            {
                _isBeepingNow = false;
                _timer->stop();
                //make sure we stop it, in case if it was in the middle of a beep.
                _buzzer->SetSpeaker(false);
            }
        }

        void BuzzerProcess::UpdateBuzzer()
        {
            //flip current state
            _buzzer->SetSpeaker(!_buzzer->IsSpeakerOn());
        }
    }
}
