#pragma once
#include <types.h>
#include <driver.h>

class MouseDriver : public CharDriver
{
public:
	MouseDriver();

	int Open(FILE *file);
	int Read(FILE *file, void *buf, size_t size);
};