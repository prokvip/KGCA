// 정해진 출력 양식(반드시 정점 위치는 SV_POSITION에 저장해야 한다.)
// 정해진 레지스터에서 정해진 레지스터로 저장한다.
// 레지스터-> x,y,z,w  9.0 -> 65545/4
Texture2D g_txDiffuse : register(t1);
SamplerState g_Sampler : register(s0);

cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
};
struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
	//float4 i : INDEX;
	//float4 w : WEIGHT;
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
	float4 vWorld = mul(vLocal, g_matWorld);
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