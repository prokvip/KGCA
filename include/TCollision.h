#pragma once
#include "TMath.h"
#include "TSelect.h"
#include "TMesh.h"

enum PointPosition
{
	CP_FRONT,   // Point is positioned in front of plane
	CP_BACK,    // Point is positioned behind plane
	CP_ONPLANE, // Point is positioned on plane
	CP_SPANNING // Point is spanning plane
};

class TCollision
{
public:
	TVector3		m_vBackPosition;
	TVector3		m_vPosition;
	TVector3		m_vCBackPosition;
	TVector3		m_vCPosition;
	TVector3		m_vCollisionTriangle[3];
	TVector3		m_vPointA;	TVector3		m_vPointB;
	TVector3		m_vPointC;	TVector3		m_vPointD;
	TVector3		m_vPointE;	TVector3		m_vPointF;
	TVector3		m_vCPointA;	TVector3		m_vCPointB;
	TVector3		m_vCPointC;	TVector3		m_vCPointD;
	TVector3		m_vCPointE;	TVector3		m_vCPointF;
	bool			m_bCollision;
	bool			m_bKeyInput;
	float			m_fUnit;
	TVector3		m_vCameraIntersection;
	TVector3		m_vCameraPos;
	using TPolygon = std::vector<TTri<TVector3>>;
	TPolygon		m_pCollisionPoly;
public:
	bool			Init();
	bool			Frame();
	bool			CameraTick();
	bool			Release();
	bool			Render();
	void			CollisionPolyRender();
	////////////////////////////////////////////////////////////////////////////
	void			SetUserPosition(TVector3 vPos);
	void			SetCollisionPoly(TPolygon& pPolygon);
	void			SetCameraPosition(TVector3 vPos) { m_vCameraPos = vPos; };
	////////////////////////////////////////////////////////////////////////////
	TVector3		GetUserPosition() { return m_vPosition; };
	TVector3		GetCameraPosition() { return m_vCameraPos; }
	bool			GetIntersection(TVector3& vStart, TVector3& vEnd, TVector3& vPlaneVertex,
		TVector3& vPlaneNormal,
		TVector3& vIntersection,
		float& fPercentage);
	bool			PointInPolygon(TVector3* vect,TVector3* faceNormal,TVector3* point1,TVector3* point2,TVector3* point3);
	bool			RayCalculation(TVector3* vPos, TVector3 vStart, TVector3 vEnd);
	bool			CollisionCheck(TVector3& vStart, TVector3& vEnd,
									TVector3* v0, TVector3* v1, TVector3* v2,
									TVector3& vPlanNormal,
									TVector3& vDirVector,
									TVector3& vIntersetion,
									float& fDistance,
									float fUnit = 0.001f);
	bool			RayIntersectToTri(TVector3& vStart, TVector3& vEnd,	TVector3& vPlaneVertex,	TVector3& vPlaneNormal,	TVector3& vIntersection,float& fPercentage);
	PointPosition	CheckPosition(TVector3 vNormal, TVector3 pPos);
public:
	BOOL	ChkBoxToRay(T_BOX& Box, T_RAY& Ray);
	BOOL	ChkBoxToRay(T_BOX& Box, TVector3 vOrigin, TVector3 vDirection);
	BOOL	ChkBoxToRay(TVector3* vBox, TVector3 vOrigin, TVector3 vDirection);
	BOOL	ChkBoxToRay(TVector3& vMax, TVector3& vMin, TVector3 vOrigin, TVector3 vDirection);
public:
	static bool  TCollision::ChkOBBToOBB(const T_BOX& box0, const T_BOX& box1);
public:
	TCollision();
	virtual ~TCollision();

};

