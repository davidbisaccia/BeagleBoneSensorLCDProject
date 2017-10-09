#include "include/Communication/SocketHandler.h"
#include <QException>
#include <QHostAddress>
#include "include/Communication/Packets/CommunicationPackets.h"

///////////////////////////////////////////////////////////


namespace BeagleBone
{
    namespace Communication
    {

        //initializing this globally
        int SocketHandler::NextID = 0;

        SocketHandler::SocketHandler(QObject* parent, QTcpSocket* socket)
            : QObject(parent), _sizeOfCurrentPacketBeingRead(-1)
        {
            _id = NextID;
            NextID++;

            _socket = socket;

            connect( _socket, SIGNAL(readyRead()), this, SLOT(HandleRead()) );
            connect( _socket, SIGNAL(disconnected()), this, SLOT(HandleDisconnect()) );
        }

        SocketHandler::~SocketHandler()
        {
            if(_socket->isOpen())
                _socket->close();
        }

        QString SocketHandler::GetIPAddress() const
        {
            if(_socket->isOpen())
            {
                QHostAddress addr = _socket->peerAddress();
                return addr.toString();
            }

            return "Socket not open.";
        }

        int SocketHandler::GetID() const
        {
            return _id;
        }

        void SocketHandler::SendMessage(PacketBase* message)
        {
            try
            {
                if(_socket->isOpen())
                {
                    QByteArray bytes = message->GetMessageBytes();
                    if(_socket->write(bytes) < 0)
                    {
                        //do we want to anything when we get an error?
                        //TODO: log?
                    }
                }
            }
            catch(QException ex)
            {
                //TODO: log?
            }
        }


        void SocketHandler::HandleRead()
        {
            QDataStream stream(_socket);
            stream.setVersion(QDataStream::Qt_5_3);

            if(_sizeOfCurrentPacketBeingRead < 0)
            {
                if(_socket->bytesAvailable() < sizeof(qint32))
                   return;

                stream >> _sizeOfCurrentPacketBeingRead;
            }

            if(_socket->bytesAvailable() < _sizeOfCurrentPacketBeingRead)
                return;

            //apparently we have enough bytes to read this message
            qint8 id;
            stream >> id;
            QByteArray data = _socket->read(_sizeOfCurrentPacketBeingRead);

            PacketBase* message = NULL;
            switch(id)
            {
                case TemperatureWaterRequestPacket::TemperatureWaterRequestPacketID:
                    message = new TemperatureWaterRequestPacket(data);
                    _sizeOfCurrentPacketBeingRead = -1;
                    break;
                case TemperatureWaterInfoPacket::TemperatureWaterInfoPacketID:
                    message = new TemperatureWaterInfoPacket(data);
                    _sizeOfCurrentPacketBeingRead = -1;
                    break;
                default: //we should not be receiving images or anything else
                    throw new QException();//"Unknown message type!");
                break;
            }

            if(message != NULL)
                emit ReceivedMessage(message, _id);
        }

        void SocketHandler::HandleDisconnect()
        {
            emit RemoveConnection(_id);
        }


    }
}
