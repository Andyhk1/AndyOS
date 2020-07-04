#pragma once
#include "types.h"
#include "PCI/pci.h"
#include "packet.h"
#include "address.h"

class NetInterface /*: public PciDevice*/
{
protected:
	PciDevice *dev;

public:
	uint32 gateway_addr;

	NetInterface(PciDevice *pci_dev);

	virtual void Send(NetPacket *pkt) {}
	virtual void Poll() {}

	//static void(*EthReceive)(NetInterface* intf, NetPacket* pkt);

	virtual MacAddress GetMac() { return MacAddress(); }
	virtual uint32 GetIP() { return 0; }
};
