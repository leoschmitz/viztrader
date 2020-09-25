//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZCTPPACKET_H__
#define __VIZCTPPACKET_H__
//-----------------------------------------------------------------------------
#include <QDataStream>

#include "VizTypes.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace net
{
//-----------------------------------------------------------------------------
class VizCTPPacket : private QByteArray, public QDataStream
{	
private:	
	VizETPServerPacket PacketType;

	void initialize();
	
public:
	//QByteArray Block;

	VizCTPPacket();
	VizCTPPacket(const VizETPServerPacket& packetType);	
	~VizCTPPacket();

	void setPacketType(const VizETPServerPacket& packetType);
	const VizETPServerPacket& packetType();

	QDataStream& data();

	QByteArray& getPacketReady();	
};
//-----------------------------------------------------------------------------
template <typename T>
static inline VizCTPPacket& operator<<(VizCTPPacket& out, const T& t)
{
	out.data() << t;	
	return out;
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
