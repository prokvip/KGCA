//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState samLinear: register (s0);
cbuffer cb0
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
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
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = vIn.c * g_MeshColor;
	return output;
}
//DUAL COLOR BLENDING
struct PS_OUT
{
	float4 add : SV_TARGET0;
	float4 mul : SV_TARGET1;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
//BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
//BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_SRC1_COLOR;
//PS_OUT PSBlock(VS_OUTPUT inData) // 바탕화면이 검은색 일 때 사용됨.
//{
//	PS_OUT vOut;
//	float4 vFinalA = g_txDiffuse.Sample(samLinear, inData.t);
//	float alpha = max(max(vFinalA.r, vFinalA.g), vFinalA.b);
//	vOut.mul = float4(1 - alpha, 1 - alpha, 1 - alpha, alpha);
//	vOut.add = vFinalA;//lerp(vFinalA, vFinalB, time.x);
//	return vOut;
//}
PS_OUT PS(VS_OUTPUT inData)
{
	PS_OUT vOut;
	float4 vFinalA = g_txDiffuse.Sample(samLinear, inData.t);
	float alpha = max(max(vFinalA.r, vFinalA.g), vFinalA.b);
	vOut.mul = float4(1 - alpha, 1 - alpha, 1 - alpha, alpha);
	vOut.add = vFinalA;//lerp(vFinalA, vFinalB, time.x);
	return vOut;
}
//
//float4 PS(VS_OUTPUT vIn) : SV_Target
//{
//	return g_txDiffuse.Sample(samLinear, vIn.t);// *vIn.c;
//}
