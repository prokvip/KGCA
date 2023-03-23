#include "Default.h"

struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

VS_OUT VS(VS_IN input) 
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(input.p,1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	float fTime = cos(g_fTimer) * 0.1f;
	float fAngle = fTime * length(input.p / 256.0f);
	float fCos, fSin;
	sincos(fAngle, fSin, fCos);
	//worldPos.x = (fCos * input.p.x - fSin*input.p.z);
	//worldPos.z = (fSin * input.p.x + fCos*input.p.z);
	vWorld.y = min(vWorld.y, vWorld.y - 2.0f - cos(fTime * input.p.x) + sin(fTime * input.p.z));


	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);	
	output.p = vProj;
	output.n = input.n;
	output.c = input.c;
	output.t.x = input.t.x;// +fTime;
	output.t.y = input.t.y;
	return output;
}

float4 PS(VS_OUT input) : SV_Target
{
	float4 vWater = g_txTex.Sample(g_SampleWrap , input.t*10.0f);
	float2 uv = input.t*10.0f;
	uv.x = 0.1f;
	uv.y = 0.1f;
	float4 vColor = vWater*g_txTex.Sample(g_SampleWrap, uv);
	vColor.a = 0.5f;
	return vColor;
}
