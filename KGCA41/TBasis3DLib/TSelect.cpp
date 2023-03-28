#include "TSelect.h"
#include "TInput.h"

void TSelect::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	Update();
}
//r(t)= o + td;
//f(x)=(x - c) dot (x - c) - r2
//f(r(t)) = (o + td - c)dot(o + td - c) - r2
//= (d dot d)t2 + ( 2 * ( (o-c) dot d ) )t + ( (o-c)dot(o-c) )-r2
//     a                    b                       c
bool TSelect::IntersectRayToSphere(T_SPHERE* pSphere, T_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	// p0 - c
	TVector3 vDir = pRay->vOrigin - pSphere->vCenter;
	// B = 2(u dot (p0 - c ))
	float  fProj =D3DXVec3Dot(&pRay->vDirection, &vDir);
	// 제한된 길이(세그먼트) 제외처리
	if (pRay->fExtent > -1.0f && pRay->fExtent <  fabs(fProj)) return false;

	float b = 2.0f * fProj;
	// C= (p0-c) dot (p0-c) - r*r
	float c = D3DXVec3Dot(&vDir, &vDir) - (pSphere->fRadius * pSphere->fRadius);
	// D = B*B - 4*AC;
	float fDiscriminant = (b*b) - (4.0f*c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);
	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection * t1;
		return true;
	}
	return false;
}
//Axis-aligned bounding box(AABB )
bool TSelect::IntersectBox(T_BOX* pBox, T_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	TVector3 tmin;
	tmin.x = (pBox->vMin.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
	tmin.y = (pBox->vMin.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmin.z = (pBox->vMin.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	TVector3 tmax;
	tmax.x = (pBox->vMax.x - pRay->vOrigin.x) / pRay->vDirection.x + 0.001f;
	tmax.y = (pBox->vMax.y - pRay->vOrigin.y) / pRay->vDirection.y + 0.001f;
	tmax.z = (pBox->vMax.z - pRay->vOrigin.z) / pRay->vDirection.z + 0.001f;

	TVector3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);
	TVector3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDirection* maxmin;
		return true;
	}
	return false;
}

bool TSelect::AABBtoRay(T_BOX* a, T_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = 0;
	float t_max = 999999.0f;
	for (int i = 0; i<3; i++)
	{
		// 축에 평행할 경우
		if (abs(pRay->vDirection[i]) < 0.0001f)
		{
			if (pRay->vOrigin[i] < a->vMin[i] || pRay->vOrigin[i] > a->vMax[i])
				return false;
		}
		else
		{
			float denom = 1.0f / pRay->vDirection[i];
			float t1 = (a->vMin[i]-pRay->vOrigin[i] ) * denom;
			float t2 = (a->vMax[i]-pRay->vOrigin[i] ) * denom;
			if (t1 > t2)
			{
				swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
	}
	m_vIntersection = pRay->vOrigin + pRay->vDirection* t_min;
	return true;
}
bool TSelect::OBBtoRay(T_BOX* pBox, T_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = -999999.0f;
	float t_max = 999999.0f;

	TVector3 diff = pBox->vCenter - pRay->vOrigin;

	for (int v = 0; v<3; v++)
	{
		float f = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection);
		float s = D3DXVec3Dot(&pBox->vAxis[v], &diff);
		
		// 축에 평행할 경우
		if (abs(pRay->vDirection[v]) < 0.0001f)
		{
			if (-s - pBox->fExtent[v] > 0 || -s + pBox->fExtent[v] > 0)
				return false;
		}
		else
		{
			float t1 = (s - pBox->fExtent[v]) / f;
			float t2 = (s + pBox->fExtent[v]) / f;
			if (t1 > t2)
			{
				swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
	}
	m_vIntersection = pRay->vOrigin + pRay->vDirection* t_min;
	return true;
}
//Oriented bounding box(OBB : RAY)
bool TSelect::ChkOBBToRay(T_BOX* pBox, T_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	TVector3 vR = pRay->vOrigin - pBox->vCenter;

	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection );
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR );
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	TVector3 vDxR;
	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[2]  분리축
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
	if (fCross[2] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}

	m_vIntersection = pRay->vOrigin + pRay->vDirection* t_min;
	return true;
}

void TSelect::Update()
{
	// Get the pick ray from the mouse position
	//if (I_Input.m_DIMouseState.rgbButtons[0])
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		// Compute the vector of the pick ray in screen space
		TVector3 v;
		// 방법 1
		/*v.x =  ( ( ( 2.0f * ptCursor.x ) / TBASIS::g_iClientWidth  ) - 1 );
		v.y = -( ( ( 2.0f * ptCursor.y ) / TBASIS::g_iClientHeight ) - 1 );
		v.z =  1.0f;
		D3DXVec3TransformCoord( &v, &v, &m );*/
		// 방법 2
		v.x = (((2.0f * ptCursor.x) / TBASIS::g_iClientWidth) - 1) / m_matProj._11;
		v.y = -(((2.0f * ptCursor.y) / TBASIS::g_iClientHeight) - 1) / m_matProj._22;
		v.z = 1.0f;

		// Get the inverse view matrix
		// 월드가 변환되었다면 버텍스버퍼도 곱해야 한다. 그러나 피킹버텍스는 월드행렬이 곱해지지 않은
		// 버텍스버퍼(노칼좌표)에서 갖고 계산하기 때문에 정점의 좌표가 다르게 된다. 
		// 결국 버텍스버퍼의 정점을 행렬로 곱해서 광선과 계산하거나( 뷰행렬만 인버스한다. )
		// 광선을 변환해야만 된다.( 뷰행렬도 인버스 해야한다.)
		// 여기서는 광선을 변환하여 동일한 좌표계로 처리하고 있다.
		// 특별한 경우가 아니라면 단위행렬를 월드행렬로 사용해라.
		TMatrix mWorldView = m_matWorld * m_matView;
		TMatrix m;
		D3DXMatrixInverse(&m, NULL, &mWorldView);

		// 방법 1
		m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
		m_Ray.vDirection = TVector3(v.x, v.y, v.z);
		D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
		D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
		D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);

		// 방법 2		
		//m_Ray.vDirection.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
		//m_Ray.vDirection.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
		//m_Ray.vDirection.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
		//D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
		//m_Ray.vOrigin.x = m._41;
		//m_Ray.vOrigin.y = m._42;
		//m_Ray.vOrigin.z = m._43;
	}
}
TVector3 TSelect::GetIntersection()
{
	return m_vIntersection;
}
// 교점을 찾는다.
bool    TSelect::GetIntersection(
	TVector3 vStart,
	TVector3 vEnd,
	TVector3 vNormal,
	TVector3 v0,
	TVector3 v1,
	TVector3 v2,
	float* fPercentage)
{
	TVector3 vDirection = vEnd - vStart;
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	float a0 = D3DXVec3Dot(&vNormal, &(v0 - vStart));
	float fT = a0 / D;
	if (fPercentage != nullptr)
	{
		*fPercentage = fT;
	}
	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}
	m_vIntersection = vStart + vDirection * fT;
	return true;
}
// 폴리곤안에 위치하는지 판단한다.
bool    TSelect::PointInPolygon(
	TVector3 vert,
	TVector3 faceNormal,
	TVector3 v0,
	TVector3 v1,
	TVector3 v2)
{
	TVector3 e0, e1, iInter, vNormal;
	// v0    v1
	//       v
	// v2    v3
	//
	//     v

	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = vert - v1;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	//     v1
	//     v
	// v0      v2
	//
	//     v
	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = vert - v2;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;
	return true;
};

bool TSelect::ChkPick(TVector3& v0, TVector3& v1, TVector3& v2)
{
	FLOAT fBary1, fBary2;
	FLOAT fDist;
	// Check if the pick ray passes through this point
	if (IntersectTriangle(m_Ray.vOrigin, m_Ray.vDirection, v0, v1, v2,
		&fDist, &fBary1, &fBary2))
	{
		m_vSrcVex[0] = v0;
		m_vSrcVex[1] = v1;
		m_vSrcVex[2] = v2;
		m_matWorldPick = m_matWorld;
		return true;
	}

	return false;
}
bool TSelect::IntersectTriangle(const TVector3& orig, const TVector3& dir,
	TVector3& v0, TVector3& v1, TVector3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	TVector3 edge1 = v1 - v0;
	TVector3 edge2 = v2 - v0;
	
	// Begin calculating determinant - also used to calculate U parameter
	//TVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	//TVector3 tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	//TVector3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	//TVector3 e3 = v2 - v1;
	//TVector3 vi = orig + (*t) * dir;
	//TVector3 i0 = v0 + edge1 * (*u) + edge2 * (*v);
	//
	//this->vPickRayOrig = orig;
	//this->vPickRayDir = dir;
	return true;
}
TSelect::TSelect(void)
{
	m_vSrcVex[0] = TVector3(0.0f, 0.0f, 0.0f);
	m_vSrcVex[1] = TVector3(0.0f, 0.0f, 0.0f);
	m_vSrcVex[2] = TVector3(0.0f, 0.0f, 0.0f);
	m_fPickDistance = 10000.0f;

	m_vIntersection = TVector3(0, 0, 0);
	m_vCenter = TVector3(0, 0, 0);
	m_vDxR = TVector3(1, 1, 1);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

TSelect::~TSelect(void)
{
}
