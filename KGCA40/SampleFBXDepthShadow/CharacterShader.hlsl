// 정해진 출력 양식(반드시 정점 위치는 SV_POSITION에 저장해야 한다.)
// 정해진 레지스터에서 정해진 레지스터로 저장한다.
// 레지스터-> x,y,z,w  9.0 -> 65545/4
#define MAX_BONE_MATRICES 255
Texture2D g_txDiffuse : register(t0);
Texture2D g_txShadow  : register(t1);
SamplerState g_Sampler : register(s0);
SamplerState g_SamplerClamp : register(s1);

cbuffer cbData: register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	matrix g_matNormal	: packoffset(c12);
	float  g_fTimer : packoffset(c16.z);
};
cbuffer cbAnimMatrix : register(b1)
{
	matrix g_matBondMatrix[MAX_BONE_MATRICES];
}
struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
	float4 i : INDEX;
	float4 w : WEIGHT;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 s : TEXCOORD1;
};
VS_OUT VS(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(vIn.p, 1.0f);
	
	float4 vAnim = 0;
	for (int i = 0; i < 4; i++)
	{
		uint iBoneIndex = (uint)vIn.i[i];
		matrix matBone = g_matBondMatrix[iBoneIndex];
		vAnim += vIn.w[i] * mul(vLocal, matBone);
	}	
	float4 vWorld = mul(vAnim, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	float4 vShadowProj = mul(vWorld, g_matNormal);
	output.s = vShadowProj;
	output.p = vProj;
	output.n = vIn.n;
	float depth = vProj.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	output.c = float4(depth, depth, depth, 1);
	output.t = vIn.t;
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	float3 vShadowProj;
	vShadowProj.xyz = v.s.xyz / v.s.w;
	vShadowProj.z = v.s.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	float shadow = g_txShadow.Sample(g_SamplerClamp,vShadowProj.xy);
	float4 color = g_txDiffuse.Sample(g_Sampler, v.t);
	if (shadow + 0.0065f <= vShadowProj.z)
	{
		color = color*float4(0.9f,0.9f,0.9f,1);
	}
	return color;
}
float4 PSShadow(VS_OUT v) : SV_TARGET
{
	return v.c;
}
