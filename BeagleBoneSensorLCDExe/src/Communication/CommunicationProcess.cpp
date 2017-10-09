#include "include/Communication/CommunicationProcess.h"
#include "include/Communication/Packets/CommunicationPackets.h"
#include <QException>
#include <QBuffer>

namespace BeagleBone
{
    namespace Communication
    {

        CommunicationProcess::CommunicationProcess(QObject* parent)
            :QThread(parent), _stop(true), _receivedInitialState(false), _temperature(0), _waterDetected(false)
        {
            //I do not think we have to new this, I am thinking too much like C# doh!
            //_sockets = new QList<SocketHandler*>;
            _server = new QTcpServer(this);
            connect( _server, SIGNAL(newConnection()), this, SLOT(NewConnection()) );
            connect( _server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(ServerError(QAbstractSocket::SocketError)) );
        }

        CommunicationProcess::~CommunicationProcess()
        {
            _stop = true;

            for(int i = 0; i < _sockets.size(); i++)
                delete _sockets[i];

            _sockets.clear();
            _server->close();
        }

        void CommunicationProcess::run()
        {
            _stop = false;

            if(!_server->isListening())
            {
                _server->listen(QHostAddress::LocalHost, 60999);
                emit SendStatus("Listening...");
            }
        }

        void CommunicationProcess::stop()
        {
            _stop = true;
            _server->close();
        }

        void CommunicationProcess::NewConnection()
        {
            // need to grab the socket
            QTcpSocket *socket = _server->nextPendingConnection();

            if(_stop)
            {
                socket->close();
                return;
            }


            QHostAddress addr = socket->peerAddress();
            emit SendStatus("Connected: " + addr.toString());

            SocketHandler* handler = new SocketHandler(this, socket);
            connect( handler, SIGNAL(RemoveConnection(int)), this, SLOT(DropConnection(int)) );
            connect( handler, SIGNAL(ReceivedMessage(PacketBase*,int)), this, SLOT(ProcessMessage(PacketBase*,int)) );

            _sockets.append(handler);
        }

        void CommunicationProcess::ServerError(QAbstractSocket::SocketError error)
        {
            emit SendStatus("Error");
        }

        void CommunicationProcess::DropConnection(const int& id)
        {
            for(int i = 0; i < _sockets.size(); i++)
            {
                if(_sockets[i]->GetID() == id)
                {
                    SocketHandler* temp = _sockets[i];
                    _sockets.removeAt(i);
                    delete temp;

                    emit SendStatus("Lost a connection");
                    return;
                }
            }
        }

        void CommunicationProcess::ReceiveTemperatureData(const int& temperature)
        {
            _temperature = temperature;
            _receivedInitialState = true;
        }

        void CommunicationProcess::ReceiveWaterDetectedStatus(const bool& detected)
        {
            _waterDetected = detected;
        }

        void CommunicationProcess::ProcessMessage(PacketBase* message, const int& id)
        {
            if(_stop || !_receivedInitialState)
                return;

            if(message->PacketID == TemperatureWaterRequestPacket::TemperatureWaterRequestPacketID)
            {
                TemperatureWaterRequestPacket* handshake = (TemperatureWaterRequestPacket*)message;
                if(handshake == NULL)
                {
                    delete handshake;
                    return;
                }

                if(handshake->CurrentVersion != handshake->version)
                {
                    delete handshake;
                    return;
                }

                TemperatureWaterInfoPacket returnMessage(_temperature, _waterDetected);
                for(int idx = 0; idx < _sockets.size(); idx++)
                {
                    if(_sockets[idx] != NULL && _sockets[idx]->GetID() == id)
                    {
                        _sockets[idx]->SendMessage(&returnMessage);
                        QString addr = _sockets[idx]->GetIPAddress();
                        emit SendStatus("Received Handshake: " + addr);
                        delete handshake;
                        return;
                    }
                }

                delete handshake;
            }
        }

    }
}
