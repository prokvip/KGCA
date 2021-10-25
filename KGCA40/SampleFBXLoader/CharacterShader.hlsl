// ������ ��� ���(�ݵ�� ���� ��ġ�� SV_POSITION�� �����ؾ� �Ѵ�.)
// ������ �������Ϳ��� ������ �������ͷ� �����Ѵ�.
// ��������-> x,y,z,w  9.0 -> 65545/4
#define MAX_BONE_MATRICES 255
Texture2D g_txDiffuse : register(t1);
SamplerState g_Sampler : register(s0);

cbuffer cbData: register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
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
	output.p = vProj;
	output.n = vIn.n;
	output.c = vIn.c;
	output.t = vIn.t;
	return output;
}
float4 PS(VS_OUT v) : SV_TARGET
{
	return g_txDiffuse.Sample(g_Sampler, v.t) *v.c;
}