//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-variable-syntax
//#pragma pack_matrix( row_major ) //column_major이 디폴트 값이다.

Texture2D g_txDiffuse: register (t0);
SamplerState samLinear: register (s0);
cbuffer cb0
{
	/*column_major*/ matrix	g_matWorld		: packoffset(c0);
	/*column_major*/ matrix	g_matView		: packoffset(c4);
	/*column_major*/ matrix	g_matProj		: packoffset(c8);
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
//https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-mul
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//////  mul(a,b)에서 b에 벡터가 오면 전치된다.( 행렬은 그대로 사용된다)
	// row_major + 비 전치된 행렬
	/*output.p = mul(g_matWorld, vIn.p);
	output.p = mul(g_matView, output.p);
	output.p = mul(g_matProj, output.p);*/
	//  column_major + 전치된 행렬
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	// row_major + 비 전치된 행렬
	/*output.p = mul(vIn.p, transpose(g_matWorld));
	output.p = mul(output.p, transpose(g_matView));
	output.p = mul(output.p, transpose(g_matProj));*/
	// column_major + 전치된 행렬
	/*output.p = mul(transpose(g_matWorld), vIn.p);
	output.p = mul(transpose(g_matView),output.p);
	output.p = mul(transpose(g_matProj),output.p);*/
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
