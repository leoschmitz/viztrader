//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2010 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#include "VizCTPPacket.h"
//-----------------------------------------------------------------------------
namespace viz
{
namespace net
{
//-----------------------------------------------------------------------------
VizCTPPacket::VizCTPPacket() : QByteArray(), QDataStream(&(*this), QIODevice::WriteOnly), PacketType(VETPSP_INVALID)
{
	initialize();
}
//-----------------------------------------------------------------------------
VizCTPPacket::VizCTPPacket(const VizETPServerPacket& packetType) : PacketType(packetType),
	QByteArray(), QDataStream(&(*this), QIODevice::WriteOnly)
{
	initialize();

	data() << PacketType;
}
//-----------------------------------------------------------------------------
VizCTPPacket::~VizCTPPacket()
{

}
//-----------------------------------------------------------------------------
void VizCTPPacket::initialize()
{
	setVersion(QDataStream::Qt_4_0);
	data() << (quint16) 0;	
}
//-----------------------------------------------------------------------------
void VizCTPPacket::setPacketType(const VizETPServerPacket& packetType)
{
	PacketType = packetType;

	data() << PacketType;
}
//-----------------------------------------------------------------------------
const VizETPServerPacket& VizCTPPacket::packetType()
{
	return PacketType;
}
//-----------------------------------------------------------------------------
QDataStream& VizCTPPacket::data()
{
	return *this;
}
//-----------------------------------------------------------------------------
QByteArray& VizCTPPacket::getPacketReady()
{
	device()->seek(0);
	data() << (quint16)((*this).size() - sizeof(quint16));

	return (*this);
}
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------