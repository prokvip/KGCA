#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "TVector.h"
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

// ȭ����ǥ��
struct TRect
{
	TVector2  vPivot;
	TVector2  vMin;
	TVector2  vMax;
	TVector2  vSize;
	TVector2  vCenter;
	TRect()
	{
		vPivot = { 0,0 };
		vMin = { 0,0 };
		vMax = { 0,0 };
		vSize = { 0,0 };
		vCenter = { 0,0 };
	}
	TRect(TVector2 vMin, TVector2 vMax)
	{
		vCenter = (vMax + vMin) / 2.0f;
		vSize = vMax - vCenter;
		TVector2 vHalf = vSize / 2.0f;
		vPivot = { vCenter.x - vHalf.x, vCenter.y - vHalf.y };
		this->vMin = vMin;
		this->vMax = vMax;
	}
};
// ������ǥ��
struct TBox
{
	TVector3  vPivot;
	TVector3  vMin;
	TVector3  vMax;
	TVector3  vSize;
	TVector3  vCenter;
	TBox()
	{
		vPivot = { 0,0,0 };
		vMin = { 0,0,0 };
		vMax = { 0,0,0 };
		vSize = { 0,0,0 };
		vCenter = { 0,0,0 };
	}
	TBox(TVector3 vMin, TVector3 vMax)
	{
		vCenter = (vMax + vMin) / 2.0f;
		vSize = vMax - vCenter;
		TVector3 vHalf = vSize / 2.0f;
		vPivot = { vCenter.x - vHalf.x, vCenter.y - vHalf.y, vCenter.z + vHalf.z };
		this->vMin = vMin;
		this->vMax = vMax;
	}
};
struct TLine
{
	// y = mx+b
	TVector3 vP1;
	float    fSlope;
};
struct TSegment
{
	TVector3 vP1;
	TVector3 vP2;	
};
struct TRay
{
	TVector3 vStart;	
	TVector3 vDirection;
};
struct TSphere 
{
	TVector3 vCenter;
	float    fRadius;
};
struct TCircle
{
	TVector2 vCenter;
	float    fRadius;
};