#pragma once
#include "definitions.h"

#define PAGE_SIZE		0x1000

#define PAGES_PER_TABLE	1024
#define PAGES_PER_DIR	1024

//typedef uint32 PAGE_TABLE_ENTRY;
//typedef uint32 PAGE_DIR_ENTRY;

enum PAGE_TABLE_FLAG
{
	PTE_PRESENT = 1,
	PTE_WRITABLE = 2,
	PTE_USER = 4,
	PTE_WRITETHROUGH = 8,
	PTE_CACHE_DISABLED = 0x10,
	PTE_ACCESSED = 0x20,
	PTE_DIRTY = 0x40,
	//PTE_PAT = 0x80,
	PTE_CPU_GLOBAL = 0x100,
	//PTE_LV4_GLOBAL = 0x200,
	PTE_FRAME = 0x7FFFF000
};

enum PAGE_DIR_FLAG
{
	PDE_PRESENT = 1,
	PDE_WRITABLE = 2,
	PDE_USER = 4,
	PDE_WRITETHROUGH = 8,
	PDE_CACHE_DISABLED = 0x10,
	PDE_ACCESSED = 0x20,
	PDE_DIRTY = 0x40,
	PDE_4MB = 0x80,
	PDE_CPU_GLOBAL = 0x100,
	//PDE_LV4_GLOBAL = 0x200,
	PDE_FRAME = 0x7FFFF000
};

struct PAGE_TABLE;

struct PAGE_TABLE_ENTRY
{
	uint32 value = 0;

	void SetFlag(PAGE_TABLE_FLAG flag)
	{
		value |= flag;
	}

	void UnsetFlag(PAGE_TABLE_FLAG flag)
	{
		value &= ~flag;
	}

	void SetAddr(uint32 addr)
	{
		value = (value & ~PTE_FRAME) | addr;
	}
};

struct PAGE_DIR_ENTRY
{
	uint32 value = 0;

	void SetFlag(PAGE_DIR_FLAG flag)
	{
		value |= flag;
	}

	void UnsetFlag(PAGE_DIR_FLAG flag)
	{
		value &= ~flag;
	}

	void SetTable(PAGE_TABLE* table)
	{
		value = (value & ~PDE_FRAME) | (uint32)table;
	}
};

struct PAGE_TABLE
{
	PAGE_TABLE_ENTRY entries[PAGES_PER_TABLE];
};

struct PAGE_DIR
{
	PAGE_DIR_ENTRY entries[PAGES_PER_DIR];
};

static class Paging
{
public:
	static STATUS Init();

	static void MapMem(PAGE_DIR* dir, uint32 phys, uint32 virt, uint32 length);

private:
	static void LoadDir(uint32 addr);
	static void EnablePaging();
};