#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H


#include <QObject>
#include <QTcpSocket>
#include <QList>

#include "include/Communication/Packets/PacketBase.h"

using namespace BeagleBone::Communication::Packets;

namespace BeagleBone
{
    namespace Communication
    {

        class SocketHandler : public QObject
        {
            Q_OBJECT

        public:
            SocketHandler(QObject* parent, QTcpSocket* socket);
            virtual ~SocketHandler();

            int GetID() const;

            static int NextID;
            void SendMessage(PacketBase* message);
            QString GetIPAddress() const;

        signals:
            void RemoveConnection(const int& id);
            void ReceivedMessage(PacketBase* message, const int& id);

        public slots:
            void HandleRead();
            void HandleDisconnect();

        private:
            int _id;
            QTcpSocket* _socket;
            qint32 _sizeOfCurrentPacketBeingRead; //will be negative when not reading anything
        };

    }
}
#endif // SOCKETHANDLER_H
