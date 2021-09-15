#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "TVector.h"
using namespace std;

struct TRect
{
	TVector3  vPivot;
	TVector3  vMin;
	TVector3  vMax;
	TVector3  vSize;
	TRect()
	{
		vPivot = { 0,0,0 };
		vMin = { 0,0,0 };
		vMax = { 0,0,0 };
		vSize = { 0,0,0 };
	}
};
