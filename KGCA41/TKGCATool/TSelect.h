#pragma once
#include "TCollision.h"

class TSelect
{
public:
	T_RAY		m_Ray;
	TVector3	m_vIntersection;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
	TVector3	m_vDxR;
	TVector3	m_vCenter;
	TVector3	m_vSrcVex[3];
	float		m_fPickDistance;
	TMatrix		m_matWorldPick;
	TVector3	qvec;
	TVector3	tvec;
	TVector3	pvec;
public:
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	void Update();
	bool IntersectRayToSphere(T_SPHERE* pSphere, T_RAY* pRay = NULL);
	bool ChkOBBToRay(T_BOX* pBox, T_RAY* pRay = NULL);
	bool IntersectBox(T_BOX* pBox, T_RAY* pRay = NULL);
	bool AABBtoRay(T_BOX* a, T_RAY* pRay = NULL);
	bool OBBtoRay(T_BOX* a, T_RAY* pRay = NULL);
	TVector3 GetIntersection();
	// 교점을 찾는다.
	bool    GetIntersection(TVector3 vStart,TVector3 vEnd,TVector3 vNormal,TVector3 v0,TVector3 v1,	TVector3 v2,float* fPercentage = nullptr);
	// 폴리곤안에 위치하는지 판단한다.
	bool    PointInPolygon(TVector3 vert,TVector3 faceNormal,TVector3 v0,TVector3 v1,TVector3 v2);
	bool	ChkPick(TVector3& v0, TVector3& v1, TVector3& v2);
	bool	IntersectTriangle(const TVector3& orig, const TVector3& dir,TVector3& v0, TVector3& v1, TVector3& v2,FLOAT* t, FLOAT* u, FLOAT* v);
public:
	TSelect(void);
	virtual ~TSelect(void);
};

