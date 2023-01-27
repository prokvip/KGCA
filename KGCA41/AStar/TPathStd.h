#pragma once
#include <windows.h>
#include <vector>
#include <list>
#include <set>
#include <iostream>
struct TIndex
{
	int x, y;
	bool  operator == (const TIndex& p)
	{
		return (x == p.x && y == p.y);
	}
	TIndex  operator + (const TIndex& p)
	{
		return { x + p.x, y + p.y };
	}
	TIndex() : x(0), y(0)
	{}
	TIndex(int fx, int fy) : x(fx), y(fy)
	{}
};