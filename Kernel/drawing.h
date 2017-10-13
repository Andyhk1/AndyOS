#pragma once
#include "definitions.h"
#include "font.h"

struct Point
{
	int x;
	int y;

	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Rect
{
	int x;
	int y;
	int width;
	int height;

	Rect()
	{
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}

	Rect(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
	}
};

struct GC
{
	int width;
	int height;
	uint32* framebuffer;

	inline int memsize()
	{
		return width * height * 4;
	}

	inline int pixels()
	{
		return width * height;
	}

	static GC CreateGraphics(int width, int height)
	{
		GC gc;
		gc.width = width;
		gc.height = height;
		gc.framebuffer = new uint32[width * height];
		return gc;
	}
};

static class Drawing
{
public:
	//static int width;
	//static int height;
	//static int memsize;

	static GC gc;
	static GC gc_direct;

	static STATUS Init(int width, int height, uint32* framebuffer);

	static void Draw(GC gc = gc);
	static void Clear(uint32 c, GC gc = gc);

	static void BitBlt(GC src, int x0, int y0, int w0, int h0, GC dst, int x1, int y1);

	static void SetPixel(int x, int y, uint32 c, GC gc = gc);

	static void DrawLine(int x0, int y0, int x1, int y1, uint32 c, GC gc = gc);
	static void DrawBezierQuad(Point* points, int count, GC gc = gc);
	static void DrawBezierCube(Point* points, int count, GC gc = gc);

	static void DrawRect(int x, int y, int w, int h, uint32 c, GC gc = gc);

	static void DrawText(int x, int y, char* c, uint32 col, GC gc = gc);
	static void DrawText(int x, int y, char* c, uint32 col, uint32 bg, GC gc = gc);
};