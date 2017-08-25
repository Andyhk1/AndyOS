#pragma once
#include "definitions.h"
#include "idt.h"

#define PIT_FREQUENCY 1000

#define	PIT_REG_COUNTER0		0x40
#define	PIT_REG_COUNTER1		0x41
#define	PIT_REG_COUNTER2		0x42
#define	PIT_REG_COMMAND			0x43

#define	PIT_OCW_MASK_BINCOUNT		1		//00000001
#define	PIT_OCW_MASK_MODE			0xE		//00001110
#define	PIT_OCW_MASK_RL				0x30	//00110000
#define	PIT_OCW_MASK_COUNTER		0xC0	//11000000

#define	PIT_OCW_BINCOUNT_BINARY		0		//0
#define	PIT_OCW_BINCOUNT_BCD		1		//1

#define	PIT_OCW_MODE_TERMINALCOUNT	0		//0000
#define	PIT_OCW_MODE_ONESHOT		0x2		//0010
#define	PIT_OCW_MODE_RATEGEN		0x4		//0100
#define	PIT_OCW_MODE_SQUAREWAVEGEN	0x6		//0110
#define	PIT_OCW_MODE_SOFTWARETRIG	0x8		//1000
#define	PIT_OCW_MODE_HARDWARETRIG	0xA		//1010

#define	PIT_OCW_RL_LATCH			0			//000000
#define	PIT_OCW_RL_LSBONLY			0x10		//010000
#define	PIT_OCW_RL_MSBONLY			0x20		//100000
#define	PIT_OCW_RL_DATA				0x30		//110000

#define	PIT_OCW_COUNTER_0			0		//00000000
#define	PIT_OCW_COUNTER_1			0x40	//01000000
#define	PIT_OCW_COUNTER_2			0x80	//10000000

static class PIT
{
public:
	static uint32 ticks;

	static STATUS Init();
	static void Sleep(uint32 time);

private:
	static STATUS Start();

	static uint8 ReadData(uint8 counter);
	static void SendData(uint16 data, uint8 counter);

	static void _cdecl PIT_ISR();
};