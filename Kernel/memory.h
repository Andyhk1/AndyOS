#pragma once
#include "definitions.h"

#define BLOCK_SIZE 0x1000

static class Memory
{
public:
	static uint32 num_blocks;
	static uint32 num_free;

	static uint32 mem_size;

	static STATUS Init(uint32 size, uint32* map);
	static void InitRegion(void* addr, uint32 size);
	static void DeInitRegion(void* addr, uint32 size);
	static void* AllocBlocks(uint32 size);
	static void FreeBlocks(void* addr, uint32 size);

	//mem size, free, used

private:
	static uint32* mem_map;

	static inline void UnsetBit(uint32 bit);
	static inline void SetBit(uint32 bit);
	static inline uint8 GetBit(uint32 bit);

	static uint32 FirstFree();
	static uint32 FirstFreeNum(uint32 size);
};