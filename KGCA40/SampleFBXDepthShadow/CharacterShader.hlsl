
cbuffer cbData: register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	matrix g_matNormal	: packoffset(c12);
	float4 g_vLightDir : packoffset(c16);
	float  g_fTimer : packoffset(c17.z);
};
#define MAX_BONE_MATRICES 255
Texture2D g_txDiffuse : register(t0);
Texture2D g_txShadow  : register(t1);
SamplerState g_Sampler : register(s0);
SamplerState g_SamplerClamp : register(s1);

cbuffer cbAnimMatrix : register(b1)
{
	matrix g_matBondMatrix[MAX_BONE_MATRICES];
}
cbuffer cbDataShadow : register(b2)
{
	matrix g_matShadow	: packoffset(c0);
};
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
	float3 vLocalNormal = vIn.n;
	float4 vAnim = 0;
	float3 vNormal = 0;
	for (int i = 0; i < 4; i++)
	{
		uint iBoneIndex = (uint)vIn.i[i];
		matrix matBone = g_matBondMatrix[iBoneIndex];
		vAnim += vIn.w[i] * mul(vLocal, matBone);
		vNormal += vIn.w[i] * mul(vLocalNormal, (float3x3)matBone);
	}	
	float4 vWorld = mul(vAnim, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	float4 vShadowProj = mul(vWorld, g_matShadow);
	output.s = vShadowProj;
	output.p = vProj;
	matrix matNormal = transpose(g_matNormal);
	output.n = normalize(mul(vNormal, (float3x3)matNormal));
	float depth = vProj.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	output.c = float4(depth, depth, depth, 1);
	output.t = vIn.t;
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	
	float4 vAmbientColor = float4(0.1f,0.1f,0.1f,1.0f);
	float4 vDiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 vLight = float3(g_vLightDir.x, g_vLightDir.y, g_vLightDir.z);
	float fDot = max(0, dot(v.n, -vLight));
	vDiffuse = vAmbient + vDiffuse * fDot;
	float3 vShadowProj;
	vShadowProj.xyz = v.s.xyz / v.s.w;
	vShadowProj.z = v.s.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	float shadow = g_txShadow.Sample(g_SamplerClamp,vShadowProj.xy);
	float4 vFinalColor = g_txDiffuse.Sample(g_Sampler, v.t);
	if (shadow + 0.0065f <= vShadowProj.z)
	{
		fDot = min(fDot, 0.5f);
	}
	vFinalColor += vAmbientColor + vDiffuseColor * fDot;
	vFinalColor.a = 1.0f;
	return vFinalColor;
}
float4 PSShadow(VS_OUT v) : SV_TARGET
{
	return v.c;
}
