#ifndef COMMUNICATIONPACKETS_H
#define COMMUNICATIONPACKETS_H

#include <QByteArray>
#include <QTcpSocket>
#include "PacketBase.h"

namespace BeagleBone
{
    namespace Communication
    {
        namespace Packets
        {

            class TemperatureWaterRequestPacket : public PacketBase
            {
            public:
                TemperatureWaterRequestPacket(QByteArray bytes);
                TemperatureWaterRequestPacket(qint8 ver);

                virtual QByteArray GetMessageBytes();
                virtual qint32 GetMessageSize();

                static const qint8 TemperatureWaterRequestPacketID = 1;
                static const qint8 CurrentVersion = 1;

                qint8 version;
            };

            class TemperatureWaterInfoPacket : public PacketBase
            {
            public:
                TemperatureWaterInfoPacket(QByteArray bytes);
                TemperatureWaterInfoPacket(qint32 temperature, bool water);

                virtual QByteArray GetMessageBytes();
                virtual qint32 GetMessageSize();

                static const qint8 TemperatureWaterInfoPacketID = 2;

                qint32 temperature;
                bool waterDetected;
            };

        }
    }
}

#endif // COMMUNICATIONPACKETS_H
