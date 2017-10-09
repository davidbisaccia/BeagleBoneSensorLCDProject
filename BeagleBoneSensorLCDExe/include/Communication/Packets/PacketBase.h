#ifndef PACKETBASE_H
#define PACKETBASE_H

#include <QByteArray>

namespace BeagleBone
{
    namespace Communication
    {
        namespace Packets
        {


            class PacketBase
            {
            public:
                PacketBase();
                virtual ~PacketBase();
                qint8 PacketID;

                virtual QByteArray GetMessageBytes() = 0;
                virtual qint32 GetMessageSize() = 0;
            };

        }
    }
}
#endif // PACKETBASE_H
