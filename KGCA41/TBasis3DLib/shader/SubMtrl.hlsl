//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
Texture2D g_txDiffuseArray[16] : register(t1);
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
	output.p = mul(g_matView, output.p);
	output.p = mul(g_matProj, output.p);
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = vIn.c;// *g_MeshColor;
	return output;
}
float4 GetTextureColor(uint index, float2 uv)
{
	float4 fColor = float4(1, 1, 1, 1);
	switch (index)
	{
	case 0: fColor = g_txDiffuseArray[0].Sample(samLinear, uv); break;
	case 1: fColor = g_txDiffuseArray[1].Sample(samLinear, uv); break;
	case 2: fColor = g_txDiffuseArray[2].Sample(samLinear, uv); break;
	case 3: fColor = g_txDiffuseArray[3].Sample(samLinear, uv); break;
	case 4: fColor = g_txDiffuseArray[4].Sample(samLinear, uv); break;
	case 5: fColor = g_txDiffuseArray[5].Sample(samLinear, uv); break;
	case 6: fColor = g_txDiffuseArray[6].Sample(samLinear, uv); break;
	case 7: fColor = g_txDiffuseArray[7].Sample(samLinear, uv); break;
	case 8: fColor = g_txDiffuseArray[8].Sample(samLinear, uv); break;
	case 9: fColor = g_txDiffuseArray[9].Sample(samLinear, uv); break;
	case 10: fColor = g_txDiffuseArray[10].Sample(samLinear, uv); break;
	case 11: fColor = g_txDiffuseArray[11].Sample(samLinear, uv); break;
	case 12: fColor = g_txDiffuseArray[12].Sample(samLinear, uv); break;
	case 13: fColor = g_txDiffuseArray[13].Sample(samLinear, uv); break;
	case 14: fColor = g_txDiffuseArray[14].Sample(samLinear, uv); break;
	case 15: fColor = g_txDiffuseArray[15].Sample(samLinear, uv); break;	
	}
	return fColor;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float4 vVColor = vIn.c;
	vVColor.a = 1.0f;
	return GetTextureColor(vIn.c.a, vIn.t) *vVColor;
}
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
	return vIn.c;
}
