#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "XVector.h"
using namespace std;

#define TBASIS_EPSILON		((float)  0.001f)
#define TBASIS_PI			((float)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

static const float T_EPSILON	= 0.001f;
static const float T_PI			= 3.141592654f;

// È­¸éÁÂÇ¥°è
struct TRect
{
	XVector2  vPivot;
	XVector2  vMin;
	XVector2  vMax;
	XVector2  vSize;
	XVector2  vCenter;
	TRect()
	{
		vPivot = { 0,0 };
		vMin = { 0,0 };
		vMax = { 0,0 };
		vSize = { 0,0 };
		vCenter = { 0,0 };
	}
	TRect(XVector2 vMin, XVector2 vMax)
	{
		vCenter = (vMax + vMin) / 2.0f;
		vSize = vMax - vCenter;
		XVector2 vHalf = vSize / 2.0f;
		vPivot = { vCenter.x - vHalf.x, vCenter.y - vHalf.y };
		this->vMin = vMin;
		this->vMax = vMax;
	}
};
// Á÷°¢ÁÂÇ¥°è
struct TBox
{
	XVector3  vPivot;
	XVector3  vMin;
	XVector3  vMax;
	XVector3  vSize;
	XVector3  vCenter;
	TBox()
	{
		vPivot = { 0,0,0 };
		vMin = { 0,0,0 };
		vMax = { 0,0,0 };
		vSize = { 0,0,0 };
		vCenter = { 0,0,0 };
	}
	TBox(XVector3 vMin, XVector3 vMax)
	{
		vCenter = (vMax + vMin) / 2.0f;
		vSize = vMax - vCenter;
		XVector3 vHalf = vSize / 2.0f;
		vPivot = { vCenter.x - vHalf.x, vCenter.y - vHalf.y, vCenter.z + vHalf.z };
		this->vMin = vMin;
		this->vMax = vMax;
	}
};
struct TLine
{
	// y = mx+b
	XVector3 vP1;
	float    fSlope;
};
struct TSegment
{
	XVector3 vP1;
	XVector3 vP2;	
};
struct TRay
{
	XVector3 vStart;	
	XVector3 vDirection;
};
struct TSphere 
{
	XVector3 vCenter;
	float    fRadius;
};
struct TCircle
{
	XVector2 vCenter;
	float    fRadius;
};