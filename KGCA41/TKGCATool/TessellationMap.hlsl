#include "Default.h"
struct HS_CONSTANT_OUTPUT
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
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
HS_CONSTANT_OUTPUT PatchConstantFunction(
	InputPatch<VS_OUT, 3> inputPatch,
	uint patchId : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT output;
	output.edges[0] = 3;
	output.edges[1] = 3;
	output.edges[2] = 3;
	output.inside = g_fTessellationAmount;
	return output;
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
	// �߿�:  mul �Լ��� �������� ó���ȴ�.
	// vWolrd.x = vLocal dot c0; // ��*��
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
	// ���� ��ǥ�� �������� �ؽ�ó ��ǥ ����
	output.t2.x = (vWorld.x + 32.0f * 1.0f) / 65.0f; // 0 ~1
	output.t2.y = 1.0f - ((vWorld.z / 32.0f * 1.0f) * 0.5f + 0.5f);//0~1	
	output.t2.x = output.t2.x * 10.0f;
	output.t2.y = output.t2.y * 10.0f;

	// �ؽ�ó ��ȯ ��� ���
	matrix matTex = 0;
	matTex._11 = 1.0f / ((65.0f / 10.0f) * 1.0f);
	matTex._32 = -1.0f / ((65.0f / 10.0f) * 1.0f);
	matTex._41 = 0.0f; // Ÿ�Ͽ� ���� Ȧ��=0.5f, ¦��=-0.0f
	matTex._42 = 0.0f;

	float4 vUV = mul(vWorld, matTex);
	output.t2.x = vUV.x;
	output.t2.y = vUV.y;

	// ������ǥ ���
	output.t2.z = (vProj.x / vProj.w) * 0.5f + 0.5f;
	output.t2.w = (vProj.y / vProj.w) * 0.5f + 0.5f;


	output.w = vWorld.xyz;
	output.n = mul(input.n.xyz, matNormal);


	// ���̸��� �ؽ�ó��ǥ
	output.TexShadow = mul(vWorld, g_matShadow);
	return output;
}
[domain("tri")]
DS_OUT DS(HS_CONSTANT_OUTPUT input,
	float3 BarycentricCoordinates : SV_DomainLocation,
	const OutputPatch<HS_OUT, 3> patch)
{
	float3 vertexPosition;
	VS_IN newVertex;

	newVertex.p =	BarycentricCoordinates.x * patch[0].p +
					BarycentricCoordinates.y * patch[1].p +
					BarycentricCoordinates.z * patch[2].p;

	newVertex.t =	BarycentricCoordinates.x * patch[0].t +
				BarycentricCoordinates.y * patch[1].t +
				BarycentricCoordinates.z * patch[2].t;

	newVertex.c = BarycentricCoordinates.x * patch[0].c +
		BarycentricCoordinates.y * patch[1].c +
		BarycentricCoordinates.z * patch[2].c;

	newVertex.n = BarycentricCoordinates.x * patch[0].n +
		BarycentricCoordinates.y * patch[1].n +
		BarycentricCoordinates.z * patch[2].n;

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
	

	float4 vBaseColor = g_txTex.Sample(g_SampleWrap , input.t2.xy);	
	float4 vColor =  LoadTex1Color(input.t2.xy,input.w);	
	vColor += LoadTex2Color(input.t2.xy,input.w);	
	vColor += LoadTex3Color(input.t2.xy,input.w);	
	vColor += LoadTex4Color(input.t2.xy,input.w);	
	vColor += LoadTex5Color(input.t2.xy,input.w);	

	float4 mask	  = g_txMaskTex.Sample( g_SampleWrap, input.t.xy );
    float4 vColorTile ;
	vColorTile = lerp(vBaseColor, g_txTex2.SampleLevel(g_SampleWrap, input.t.xy * 10, 0), mask.r);
    vColorTile =  lerp(vColorTile, g_txTex3.SampleLevel( g_SampleWrap, input.t.xy * 10, 0 ),mask.g );
	vColorTile = lerp(vColorTile, g_txTex4.SampleLevel(g_SampleWrap, input.t.xy * 10, 0), mask.b);
	vColorTile = lerp(vColorTile, g_txTex5.SampleLevel(g_SampleWrap, input.t.xy * 10, 0), mask.a);
 
    float4 vAmbintColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	
	float4 vLightColor = ComputePointDiffuseLight(input.w, input.n, 1)
						+ComputeSpotDiffuseLight(input.w, input.n, 1);
						+ComputePointSpecularLight(input.w, input.n, 1)
						+ComputeSpotSpecularLight(input.w, input.n, 1);
	float4 fBlendColor=0;
	fBlendColor.r = max(vColor.r, vColorTile.r);
	fBlendColor.g = max(vColor.g, vColorTile.g);
	fBlendColor.b = max(vColor.b, vColorTile.b);
	fBlendColor.a = min(vColor.a, vColorTile.a);

	float4 fFinalColor = fBlendColor *(vAmbintColor + vLightColor * fBlueDepth);// PCF �ݿ�
	//float fIntensity =1.0f-saturate(dot(input.n, -g_vLightDir.xyz));
	fFinalColor.a = 1.0f;
	return fFinalColor;
}

float4 PSColor(VS_OUT input) : SV_Target
{
	return input.c;
}