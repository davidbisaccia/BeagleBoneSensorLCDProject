#ifndef COMMUNICATIONPROCESS_H
#define COMMUNICATIONPROCESS_H


#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "include/Communication/Packets/PacketBase.h"
#include "SocketHandler.h"

using namespace BeagleBone::Communication::Packets;

namespace BeagleBone
{
    namespace Communication
    {

        class CommunicationProcess : public QThread
        {
            Q_OBJECT
        public:
            explicit CommunicationProcess(QObject *parent = NULL);
            virtual ~CommunicationProcess();

            void run();
            void stop();

        signals:
            void SendStatus(const QString& msg);

        public slots:
            void NewConnection();
            void DropConnection(const int& id);
            void ProcessMessage(PacketBase* message, const int& id);
            //void SendImage(QImage& image);
            void ServerError(QAbstractSocket::SocketError error);

            void ReceiveTemperatureData(const int& temperature);
            void ReceiveWaterDetectedStatus(const bool& detected);

        private:
            QTcpServer* _server;
            QList<SocketHandler*> _sockets;
            bool _stop;
            int _temperature;
            bool _waterDetected;

            bool _receivedInitialState;
        };

    }

}
///////////////////////////////////////////////////////////







#endif // COMMUNICATIONPROCESS_H
