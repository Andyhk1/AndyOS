#pragma once
#include "types.h"
#include "address.h"
#include "netinterface.h"
#include "packet.h"
#include "eth.h"
#include "ipv4.h"

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_REQUEST 8

struct ICMP_Header
{
	uint8 type;
	uint8 code;
	uint16 checksum;
	uint16 id;
	uint16 seq;
} __attribute__((packed));

struct ICMP_Packet
{
	ICMP_Header *header;
	uint8 *data;
	uint32 data_length;
} __attribute__((packed));

namespace ICMP
{
	void Send(NetInterface *intf, ICMP_Packet *icmp, uint32 tip, uint8 type);
	void SendReply(NetInterface *intf, ICMP_Packet *icmp, uint32 tip);

	void HandlePacket(NetInterface *intf, IPv4_Header *ip_hdr, NetPacket *pkt);
} // namespace ICMP
