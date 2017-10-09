#include "include/Communication/Packets/CommunicationPackets.h"
#include <QException>
#include <QBuffer>

///////////////////////////////////////////////////////////

namespace BeagleBone
{
    namespace Communication
    {
        namespace Packets
        {
            TemperatureWaterRequestPacket::TemperatureWaterRequestPacket(QByteArray bytes)
            {
                if(bytes.size() != GetMessageSize())
                    throw new QException();

                QDataStream stream(&bytes, QIODevice::ReadOnly);
                qint32 size;
                stream >> size; //kind of redundant
                if(size != GetMessageSize())
                    throw new QException();

                qint8 id;
                stream >> id;
                if(id != TemperatureWaterRequestPacketID)
                    throw new QException();

                stream >> version;
            }

            TemperatureWaterRequestPacket::TemperatureWaterRequestPacket(qint8 ver)
                : version(ver)
            {
            }

            qint32 TemperatureWaterRequestPacket::GetMessageSize()
            {
                return 6; //6 = (32 bit + 8bit + 8bit)
            }

            QByteArray TemperatureWaterRequestPacket::GetMessageBytes()
            {
                QByteArray buffer;
                QDataStream stream(&buffer, QIODevice::WriteOnly);

                stream << GetMessageSize() << TemperatureWaterRequestPacketID << version;

                return buffer;
            }

            ///////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////

            TemperatureWaterInfoPacket::TemperatureWaterInfoPacket(QByteArray bytes)
            {
                if(bytes.size() != GetMessageSize())
                    throw new QException();

                QDataStream stream(&bytes, QIODevice::ReadOnly);
                qint32 size;
                stream >> size; //kind of redundant
                if(size != GetMessageSize())
                    throw new QException();

                qint8 id;
                stream >> id;
                if(id != TemperatureWaterInfoPacketID)
                    throw new QException();

                stream >> temperature;
                stream >> waterDetected;
            }

            TemperatureWaterInfoPacket::TemperatureWaterInfoPacket(qint32 temperature, bool water)
            {
                this->temperature = temperature;
                this->waterDetected = water;
            }

            QByteArray TemperatureWaterInfoPacket::GetMessageBytes()
            {
                QByteArray buffer;
                QDataStream stream(&buffer, QIODevice::WriteOnly);

                stream << GetMessageSize() << TemperatureWaterInfoPacketID << temperature << waterDetected;

                return buffer;
            }

            qint32 TemperatureWaterInfoPacket::GetMessageSize()
            {
                return 10; //10 = (32 bits + 8bit + 32 bit + 8bit)
            }

        }
    }
}

/////////////////////////////////////////////////////////////////

