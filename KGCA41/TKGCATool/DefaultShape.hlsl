#include "Default.h"

struct VS_IN
{
	float3 p : POSITION; 
    float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

VS_OUT VS(VS_IN input) 
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(input.p,1.0f);
	// �߿�:  mul �Լ��� �������� ó���ȴ�.
	// vWolrd.x = vLocal dot c0; // ��*��
	// vWolrd.y = vLocal dot c1;
	// vWolrd.z = vLocal dot c2;
	// vWolrd.w = vLocal dot c3;	
	// matrix matTranspose = transpose(g_matWorld);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);	
	output.p = vProj;
	output.c = input.c;
	output.t = input.t;
	return output;
}

float4 PS(VS_OUT input) : SV_Target
{
	return input.c;
}