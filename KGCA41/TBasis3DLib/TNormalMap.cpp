#include "TNormalMap.h"

DWORD TNormalMap::EncodeVectorAsDWORDColor(TVector3* vVector)
{
	DWORD dwRed = (DWORD)(127 * vVector->x + 128);
	DWORD dwGreen = (DWORD)(127 * vVector->y + 128);
	DWORD dwBlue = (DWORD)(127 * vVector->z + 128);
	return (DWORD)(0xff000000 + (dwRed << 16) + (dwGreen << 8) + dwBlue);
}
/*
float3x3 tbnmatrix = transpose(float3x3(vTangent,vBiNormal,vNormal));
float3 lightdirtangent = mul(lightdir,tbnmatrix);
*/
void TNormalMap::CreateTangentSpaceVectors(TVector3 *v0,
	TVector3 *v1, TVector3 *v2, TVector2 uv0,
	TVector2 uv1, TVector2 uv2,
	TVector3 *vTangent, TVector3 *vBiNormal, TVector3 *vNormal)
{
	TVector3 vEdge1 = *v1 - *v0;
	TVector3 vEdge2 = *v2 - *v0;
	D3DXVec3Normalize(&vEdge1, &vEdge1);
	D3DXVec3Normalize(&vEdge2, &vEdge2);
	// UV delta
	TVector2 deltaUV1 = uv1 - uv0;
	TVector2 deltaUV2 = uv2 - uv0;
	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
	D3DXVec2Normalize(&deltaUV2, &deltaUV2);


	TVector3 biNormal;
	float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (fabsf(fDet) < 1e-6f)
	{
		*vTangent = TVector3(1.0f, 0.0f, 0.0f);
		biNormal = TVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		*vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
		biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
	}
	D3DXVec3Normalize(vTangent, vTangent);
	D3DXVec3Normalize(&biNormal, &biNormal);

	D3DXVec3Cross(vBiNormal, vNormal, vTangent);
	float crossinv = (D3DXVec3Dot(vBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
	*vBiNormal *= crossinv;
}

/*
float3x3 tbnmatrix = float3x3(vTangent,vBiNormal,vNormal);
float3 lightdirtangent = mul(lightdir,tbnmatrix);
*/
void TNormalMap::CreateTangentSpaceVectors(TVector3 *v1,
	TVector3 *v2,
	TVector3 *v3,
	float v1u, float v1v,
	float v2u, float v2v,
	float v3u, float v3v,
	TVector3 *vTangent,
	TVector3 *vBiNormal,
	TVector3 *vNormal)
{

	TVector3 vEdge1 = *v2 - *v1;
	TVector3 vEdge2 = *v3 - *v1;
	float fEdge1_U = v2u - v1u;
	float fEdge1_V = v2v - v1v;
	float fEdge2_U = v3u - v1u;
	float fEdge2_V = v3v - v1v;

	float fDenominator = fEdge1_U * fEdge2_V - fEdge2_U * fEdge1_V;

	if (fDenominator < 0.0001f && fDenominator > -0.0001f)
	{
		*vTangent = TVector3(1.0f, 0.0f, 0.0f);
		*vBiNormal = TVector3(0.0f, 1.0f, 0.0f);
		*vNormal = TVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float fScale1 = 1.0f / fDenominator;

		TVector3 T;
		TVector3 B;
		TVector3 N;

		T = TVector3((fEdge2_V * vEdge1.x - fEdge1_V * vEdge2.x) * fScale1,
			(fEdge2_V * vEdge1.y - fEdge1_V * vEdge2.y) * fScale1,
			(fEdge2_V * vEdge1.z - fEdge1_V * vEdge2.z) * fScale1);

		B = TVector3((-fEdge2_U * vEdge1.x + fEdge1_U * vEdge2.x) * fScale1,
			(-fEdge2_U * vEdge1.y + fEdge1_U * vEdge2.y) * fScale1,
			(-fEdge2_U * vEdge1.z + fEdge1_U * vEdge2.z) * fScale1);

		D3DXVec3Cross(&N, &T, &B);

		T *= fScale1;
		B *= fScale1;
		D3DXVec3Normalize(&T, &T);
		D3DXVec3Normalize(&B, &B);
		*vTangent = T;
		*vBiNormal = B;
		*vNormal = N;

		//float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
		//	(B.x * N.y * T.z - B.z * N.y * T.x) +
		//	(N.x * T.y * B.z - N.z * T.y * B.x));

		//TVector3 vTemp;

		//(*vTangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2;
		//(*vTangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		//(*vTangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		//D3DXVec3Normalize(&(*vTangent), &(*vTangent));

		//(*vBiNormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2);
		//(*vBiNormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		//(*vBiNormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		//D3DXVec3Normalize(&(*vBiNormal), &(*vBiNormal));

		//(*vNormal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2;
		//(*vNormal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		//(*vNormal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		//D3DXVec3Normalize(&(*vNormal), &(*vNormal));
		////*vBiNormal = *vBiNormal * -1.0f;
	}
}

void TNormalMap::Release()
{
}

TNormalMap::TNormalMap(void)
{
}

TNormalMap::~TNormalMap(void)
{
}

