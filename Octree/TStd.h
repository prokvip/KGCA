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
	TVector3  vCenter;
	TRect()
	{
		vPivot = { 0,0,0 };
		vMin = { 0,0,0 };
		vMax = { 0,0,0 };
		vSize = { 0,0,0 };
		vCenter = { 0,0,0 };
	}
	TRect(TVector3 vMin, TVector3 vMax)
	{
		vCenter = (vMax + vMin) / 2.0f;
		vSize = vMax - vCenter;
		TVector3 vHalf = vSize / 2.0f;
		vPivot = { vCenter.x - vHalf.x, vCenter.y - vHalf.y, vCenter.z + vHalf.z };
		this->vMin = vMin;
		this->vMax = vMax;
	}
};
