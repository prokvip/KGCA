#include "Default.h"

// Fresnel Factor
static const float3 Fdielectric = 0.04f;
static const float PI = 3.141592;
static const float Epsilon = 0.00001f;

struct HS_CONSTANT_OUTPUT1
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};
struct HS_CONSTANT_OUTPUT
{
	// Tess factor for the FF HW block
	float edges[3]    : SV_TessFactor;
	float inside : SV_InsideTessFactor;

	// Geometry cubic generated control points
	float3 f3B210    : POSITION3;
	float3 f3B120    : POSITION4;
	float3 f3B021    : POSITION5;
	float3 f3B012    : POSITION6;
	float3 f3B102    : POSITION7;
	float3 f3B201    : POSITION8;
	float3 f3B111    : CENTER;

	// Normal quadratic generated control points
	float3 f3N110    : NORMAL3;
	float3 f3N011    : NORMAL4;
	float3 f3N101    : NORMAL5;
};

struct VS_IN
{
	float3 p : POSITION; 
	float3 n : NORMAL; 
    float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct HS_OUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct DS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL; 
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 L : TEXCOORD1;
	float3 w : TEXCOORD2;//world
	float4 t2 : TEXCOORD3;
	float4 TexShadow : TEXCOORD4;
};

VS_OUT VS(VS_IN input) 
{
	VS_OUT output = (VS_OUT)0;	
	output.p = float4(input.p,1.0f);
	output.n = input.n;
	output.c = input.c;
	output.t = input.t;
	return output;
}
//HS_CONSTANT_OUTPUT1 PatchConstantFunction1(
//	InputPatch<VS_OUT, 3> inputPatch,
//	uint patchId : SV_PrimitiveID)
//{
//	HS_CONSTANT_OUTPUT1 output;
//	output.edges[0] = 3;
//	output.edges[1] = 3;
//	output.edges[2] = 3;
//	output.inside = g_fTessellationAmount;
//	return output;
//}
HS_CONSTANT_OUTPUT PatchConstantFunction(InputPatch<VS_OUT, 3> I)
{
	HS_CONSTANT_OUTPUT O = (HS_CONSTANT_OUTPUT)0;
	O.edges[0] = 3;
	O.edges[1] = 3;
	O.edges[2] = 3;

	// Assign Positions
	float3 f3B003 = I[0].p;
	float3 f3B030 = I[1].p;
	float3 f3B300 = I[2].p;
	// And Normals
	float3 f3N002 = I[0].n;
	float3 f3N020 = I[1].n;
	float3 f3N200 = I[2].n;

	// Compute the cubic geometry control points
	// Edge control points
	O.f3B210 = ((2.0f * f3B003) + f3B030 - (dot((f3B030 - f3B003), f3N002) * f3N002)) / 3.0f;
	O.f3B120 = ((2.0f * f3B030) + f3B003 - (dot((f3B003 - f3B030), f3N020) * f3N020)) / 3.0f;
	O.f3B021 = ((2.0f * f3B030) + f3B300 - (dot((f3B300 - f3B030), f3N020) * f3N020)) / 3.0f;
	O.f3B012 = ((2.0f * f3B300) + f3B030 - (dot((f3B030 - f3B300), f3N200) * f3N200)) / 3.0f;
	O.f3B102 = ((2.0f * f3B300) + f3B003 - (dot((f3B003 - f3B300), f3N200) * f3N200)) / 3.0f;
	O.f3B201 = ((2.0f * f3B003) + f3B300 - (dot((f3B300 - f3B003), f3N002) * f3N002)) / 3.0f;
	// Center control point
	float3 f3E = (O.f3B210 + O.f3B120 + O.f3B021 + O.f3B012 + O.f3B102 + O.f3B201) / 6.0f;
	float3 f3V = (f3B003 + f3B030 + f3B300) / 3.0f;
	O.f3B111 = f3E + ((f3E - f3V) / 2.0f);

	// Compute the quadratic normal control points, and rotate into world space
	float fV12 = 2.0f * dot(f3B030 - f3B003, f3N002 + f3N020) / dot(f3B030 - f3B003, f3B030 - f3B003);
	O.f3N110 = normalize(f3N002 + f3N020 - fV12 * (f3B030 - f3B003));
	float fV23 = 2.0f * dot(f3B300 - f3B030, f3N020 + f3N200) / dot(f3B300 - f3B030, f3B300 - f3B030);
	O.f3N011 = normalize(f3N020 + f3N200 - fV23 * (f3B300 - f3B030));
	float fV31 = 2.0f * dot(f3B003 - f3B300, f3N200 + f3N002) / dot(f3B003 - f3B300, f3B003 - f3B300);
	O.f3N101 = normalize(f3N200 + f3N002 - fV31 * (f3B003 - f3B300));

	// Inside tess factor is just the average of the edge factors
	O.inside = g_fTessellationAmount;// (O.edges[0] + O.edges[1] + O.edges[2]) / 3.0f;

	return O;
}
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
[maxtessfactor(64.0f)]
HS_OUT HS(		InputPatch< VS_OUT, 3> patch,
					uint pointId : SV_OutputControlPointID,
					uint patchId : SV_PrimitiveID)
{
	HS_OUT output;
	output.p = patch[pointId].p;
	output.n = patch[pointId].n;
	output.c = patch[pointId].c;
	output.t = patch[pointId].t;
	return output;
}
DS_OUT VertexWork(VS_IN input)
{
	DS_OUT output = (DS_OUT)0;
	float4 vLocal = float4(input.p, 1.0f);
	// 중요:  mul 함수는 내적으로 처리된다.
	// vWolrd.x = vLocal dot c0; // 행*행
	// vWolrd.y = vLocal dot c1;
	// vWolrd.z = vLocal dot c2;
	// vWolrd.w = vLocal dot c3;	
	matrix matNormal = transpose(g_matWorldInverse);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	output.p = vProj;
	output.c = input.c;
	output.t = input.t;

	// 0 ~ 65, cellDistange = 1.0f, tileCnt = 10.0f
	//float xMin = 32*1.0f;
	//float xMax = +32*1.0f;
	// 월드 좌표를 기준으로 텍스처 좌표 생성
	output.t2.x = (vWorld.x + 32.0f * 1.0f) / 65.0f; // 0 ~1
	output.t2.y = 1.0f - ((vWorld.z / 32.0f * 1.0f) * 0.5f + 0.5f);//0~1	
	output.t2.x = output.t2.x * 10.0f;
	output.t2.y = output.t2.y * 10.0f;

	// 텍스처 변환 행렬 사용
	matrix matTex = 0;
	matTex._11 = 1.0f / ((65.0f / 10.0f) * 1.0f);
	matTex._32 = -1.0f / ((65.0f / 10.0f) * 1.0f);
	matTex._41 = 0.0f; // 타일에 개수 홀수=0.5f, 짝수=-0.0f
	matTex._42 = 0.0f;

	float4 vUV = mul(vWorld, matTex);
	output.t2.x = vUV.x;
	output.t2.y = vUV.y;

	// 투영좌표 사용
	output.t2.z = (vProj.x / vProj.w) * 0.5f + 0.5f;
	output.t2.w = (vProj.y / vProj.w) * 0.5f + 0.5f;


	output.w = vWorld.xyz;
	output.n = mul(input.n.xyz, matNormal);


	// 깊이맵의 텍스처좌표
	output.TexShadow = mul(vWorld, g_matShadow);
	return output;
}
//[domain("tri")]
//DS_OUT DS1(HS_CONSTANT_OUTPUT1 input,
//	float3 BarycentricCoordinates : SV_DomainLocation,
//	const OutputPatch<HS_OUT, 3> patch)
//{
//	float3 vertexPosition;
//	VS_IN newVertex;
//
//	newVertex.p =	BarycentricCoordinates.x * patch[0].p +
//					BarycentricCoordinates.y * patch[1].p +
//					BarycentricCoordinates.z * patch[2].p;
//
//	newVertex.t =	BarycentricCoordinates.x * patch[0].t +
//				BarycentricCoordinates.y * patch[1].t +
//				BarycentricCoordinates.z * patch[2].t;
//
//	newVertex.c = BarycentricCoordinates.x * patch[0].c +
//		BarycentricCoordinates.y * patch[1].c +
//		BarycentricCoordinates.z * patch[2].c;
//
//	newVertex.n = BarycentricCoordinates.x * patch[0].n +
//		BarycentricCoordinates.y * patch[1].n +
//		BarycentricCoordinates.z * patch[2].n;
//
//	return VertexWork(newVertex);
//}
////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
DS_OUT DS(HS_CONSTANT_OUTPUT HSConstantData,
	const OutputPatch<HS_OUT, 3> I,
	float3 f3BarycentricCoords : SV_DomainLocation)
{
	DS_OUT O = (DS_OUT)0;
	VS_IN newVertex;
	// The barycentric coordinates
	float fU = f3BarycentricCoords.x;
	float fV = f3BarycentricCoords.y;
	float fW = f3BarycentricCoords.z;

	// Precompute squares and squares * 3 
	float fUU = fU * fU;
	float fVV = fV * fV;
	float fWW = fW * fW;
	float fUU3 = fUU * 3.0f;
	float fVV3 = fVV * 3.0f;
	float fWW3 = fWW * 3.0f;

	// Compute position from cubic control points and barycentric coords
	newVertex.p = I[0].p * fWW * fW +
		I[1].p * fUU * fU +
		I[2].p * fVV * fV +
		HSConstantData.f3B210 * fWW3 * fU +
		HSConstantData.f3B120 * fW * fUU3 +
		HSConstantData.f3B201 * fWW3 * fV +
		HSConstantData.f3B021 * fUU3 * fV +
		HSConstantData.f3B102 * fW * fVV3 +
		HSConstantData.f3B012 * fU * fVV3 +
		HSConstantData.f3B111 * 6.0f * fW * fU * fV;

	// Compute normal from quadratic control points and barycentric coords
	float3 f3Normal = I[0].n * fWW +
		I[1].n * fUU +
		I[2].n * fVV +
		HSConstantData.f3N110 * fW * fU +
		HSConstantData.f3N011 * fU * fV +
		HSConstantData.f3N101 * fW * fV;

	// Normalize the interpolated normal    
	newVertex.n = normalize(f3Normal);

	newVertex.c = f3BarycentricCoords.x * I[0].c +
		f3BarycentricCoords.y * I[0].c +
		f3BarycentricCoords.z * I[0].c;
	newVertex.c.a = 1.0f;
	newVertex.t = I[0].t * fW + I[1].t * fU + I[2].t * fV;

	return VertexWork(newVertex);
}

float4 LoadTex1Color(float2 uv, float3 vWorld) 
{
	float4 vColor = 0;
	if(vWorld.y <= 8.0f)
	{
		float4 vColorA = g_txTex.Sample(g_SampleWrap , uv);	
		float4 vColorB = g_txTex2.Sample(g_SampleWrap , uv);
		float t = smoothstep(2.0f, 4.0f, vWorld.y);
		vColor = lerp( vColorA, vColorB,t);
	}
	return vColor;
}
float4 LoadTex2Color(float2 uv, float3 vWorld) 
{
	float4 vColor = 0;
	if(vWorld.y > 8.0f && vWorld.y <= 13.0f)
	{
		float4 vColorA = g_txTex2.Sample(g_SampleWrap , uv);	
		float4 vColorB = g_txTex3.Sample(g_SampleWrap , uv);
		float t = smoothstep(8.0f, 10.0f, vWorld.y);
		vColor = lerp( vColorA, vColorB,t);	
	}
	return vColor;
}
float4 LoadTex3Color(float2 uv, float3 vWorld) 
{
	float4 vColor = 0;
	if(vWorld.y > 13.0f && vWorld.y <= 18.0f)
	{
		float4 vColorA = g_txTex3.Sample(g_SampleWrap , uv);	
		float4 vColorB = g_txTex4.Sample(g_SampleWrap , uv);
		float t = smoothstep(13.0f, 15.0f, vWorld.y);
		vColor = lerp( vColorA, vColorB,t);	
	}
	return vColor;
}
float4 LoadTex4Color(float2 uv, float3 vWorld) 
{
	float4 vColor = 0;
	if(vWorld.y > 18.0f && vWorld.y <= 25.0f)
	{
		float4 vColorA = g_txTex4.Sample(g_SampleWrap , uv);	
		float4 vColorB = g_txTex5.Sample(g_SampleWrap , uv);
		float t = smoothstep(18.0f, 20.0f, vWorld.y);
		vColor = lerp( vColorA, vColorB,t);	
	}
	return vColor;
}
float4 LoadTex5Color(float2 uv, float3 vWorld) 
{
	float4 vColor = 0;
	if(vWorld.y > 25.0f)
	{
		vColor = g_txTex5.Sample(g_SampleWrap , uv);					
	}
	return vColor;
}


float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float ndfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}
// 큐브맵의 밉맵레벨 반환
uint querySpecularTextureLevels()
{
	uint width, height, levels;
	g_txSpecularTex.GetDimensions(0, width, height, levels);
	return levels;
}
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

float4 PS(DS_OUT input) : SV_Target
{
	float3 ShadowTexColor =input.TexShadow.xyz / input.TexShadow.w;
	float  fShadowDepth = g_txDepthShadow.SampleCmpLevelZero(g_samComShadowMap, ShadowTexColor.xy, ShadowTexColor.z);	
	
	float fBlueDepth= 1.0f;
	const int g_iNumKernel = 5;
	const float fdelta = 1.0f / 16384.0f;
	int iHalf = (g_iNumKernel - 1) / 2;
	for (int v = -iHalf; v <= iHalf; v++)
	{
		for (int u = -iHalf; u <= iHalf; u++)
		{
			float2 vOffset = float2(u*fdelta, v*fdelta);	
			fBlueDepth += g_txDepthShadow.SampleCmpLevelZero(g_samComShadowMap, 
									ShadowTexColor.xy+vOffset, ShadowTexColor.z).r;
		}											
	}		
	fBlueDepth /= g_iNumKernel*g_iNumKernel;
	
	// PDB
	float3    albedo = g_txAlbedoTex.Sample(g_SampleClamp, input.t).rgb;
	float     metalness = g_txMetalnessTex.Sample(g_SampleClamp, input.t).g;
	float     roughness = g_txRoughnessTex.Sample(g_SampleClamp, input.t).b;
	float3    emisive = g_txEmisive.Sample(g_SampleClamp, input.t).rgb;	
	float3    N = g_txNormalTex.Sample(g_SampleClamp, input.t).rgb;
	float3    Env = g_txEnvTex..Sample(g_SampleClamp, input.t).rgb;

	// 월드공간
	float3 Lo = normalize(vEyePos.xyz - input.w);
	N = normalize( 2.0f * N - 1.0f);
	float cosLo = max(0.0, dot(N,Lo));
	float3 Lr = 2.0f * cosLo * N - Lo; // 반사벡터
	float3 F0 = lerp(Fdielectric, albedo, metalness);

	//pdr = BRDF + IBL
	// BRDF
	float3 directLighting = 0.0f;
	{
		// 모든 라이트 갯수 -> 종합
		float3 diffuseBRDF = 0;
		float3 specularBRDF = 0;
		for(uint i=0; i < 1; ++i)
		{
			float3 Li = -vLightDir.xyz;
			float3 Lh = normalize(Li + Lo);// 하프벡터
			float cosLi = max(0.0f, dot(N, Li));// normal dot lightdir
			float cosLh = max(0.0f, dot(N, Lh));// normal dot half
			float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo));
			float  D = ndfGGX(cosLh, roughness);
			float  G = gaSchlickGGX(cosLi, cosLo, roughness);
			float3 kd = lerp(float3(1,1,1) - F, float3(0,0,0), metalness);
			diffuseBRDF = kd * albedo;
			specularBRDF = (F*D*G) / max(Epsilon, 4.0*cosLi*cosLo);
			directLighting += (diffuseBRDF + specularBRDF) * cosLi;
		}
	}
	//IBL
	float3 ambientLighting = 0.0f;
	{
		float3 diffuseIBL = 0;
		float3 specularIBL = 0;
		// 이레이디언스맵(방사 조도맵) : 방출되는(픽쉘로부터 나오는)빛의 양을 나타내는 단위
		float3 irradiance = g_txirradiance.Sample(g_SampleClamp, N).rgb;
		float3 F = fresnelSchlick(F0,cosLo);
		float3 kd = lerp(1.0f - F, 0, metalness);
		diffuseIBL = kd * albedo *irradiance;

		uint specualrTextureLevels = querySpecularTextureLevels();
		float3 specularirradiance = g_txSpecularTex.SampleLevel(
			g_SampleClamp, Lr, roughness * specualrTextureLevels).rgb;

		float2 specularBRDF = g_txSpecularBRDF_LUT.Sample(g_SampleClamp, 
			float2(cosLi, roughness).rg;
		specularIBL = (F0 * specularBRDF.x + specularBRDF.y)* specularirradiance;
		ambientLighting = diffuseIBL + specularIBL;
		
	}
	// 노말매핑 -> 정점쉐이더 텍스처좌표 공간으로 변환->픽셀쉐이더 전달
	// 노말매핑 -> 정점쉐이더 모델좌표공간(월드) 변환 행렬->픽셀쉐이더 전달해서 월드변환
	float4 fFinalColor = (directLighting+ ambientLighting) * fBlueDepth);
	fFinalColor.a = 1.0f;
	return fFinalColor;
}

float4 PSColor(VS_OUT input) : SV_Target
{
	return input.c;
}