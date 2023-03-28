//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState samLinear: register (s0);
cbuffer cb0
{
	row_major float4x4	g_matWorld	: packoffset(c0);
	row_major float4x4	g_matView		: packoffset(c4);
	row_major float4x4	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(g_matWorld, vIn.p);
	output.p = mul(g_matView,output.p );
	output.p = mul(g_matProj,output.p );
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = vIn.c * g_MeshColor;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	return g_txDiffuse.Sample(samLinear, vIn.t) * vIn.c;
}
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
	return vIn.c;
}
