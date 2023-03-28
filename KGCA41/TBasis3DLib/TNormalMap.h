#pragma once
#include "TShape.h"

class TNormalMap
{
public:
	DWORD EncodeVectorAsDWORDColor(TVector3* vVector);
	void CreateTangentSpaceVectors(TVector3 *v0,
		TVector3 *v1,
		TVector3 *v2,
		TVector2 uv0,
		TVector2 uv1,
		TVector2 uv2,
		TVector3 *vTangent,
		TVector3 *vBiNormal,
		TVector3 *vNormal);
	void CreateTangentSpaceVectors(TVector3 *v1,
		TVector3 *v2,
		TVector3 *v3,
		float v1u, float v1v,
		float v2u, float v2v,
		float v3u, float v3v,
		TVector3 *vTangent,
		TVector3 *vBiNormal,
		TVector3 *vNormal);
	void Release();
public:
	TNormalMap(void);
	~TNormalMap(void);
};
