#ifndef BUZZERPROCESS_H
#define BUZZERPROCESS_H

#endif // BUZZERPROCESS_H

#include <QtCore>
#include <QTimer>

#include "include/Sound/Buzzer.h"

namespace BeagleBone
{
    namespace Controllers
    {
        class BuzzerProcess : public QThread
        {
            Q_OBJECT

        public:
            explicit BuzzerProcess(QObject* parent = 0);
            virtual ~BuzzerProcess();

            void Stop();

        public slots:
            void Beep(bool soundOn);

        private slots:
            void UpdateBuzzer();

        private:
            bool _isBeepingNow;
            QTimer* _timer;
            Sound::Buzzer* _buzzer;
        };
    }
}
