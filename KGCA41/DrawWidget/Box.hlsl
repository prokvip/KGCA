//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-variable-syntax
//#pragma pack_matrix( row_major ) //column_major�� ����Ʈ ���̴�.

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
	//////  mul(a,b)���� b�� ���Ͱ� ���� ��ġ�ȴ�.( ����� �״�� ���ȴ�)
	// row_major + �� ��ġ�� ���
	/*output.p = mul(g_matWorld, vIn.p);
	output.p = mul(g_matView, output.p);
	output.p = mul(g_matProj, output.p);*/
	//  column_major + ��ġ�� ���
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	// row_major + �� ��ġ�� ���
	/*output.p = mul(vIn.p, transpose(g_matWorld));
	output.p = mul(output.p, transpose(g_matView));
	output.p = mul(output.p, transpose(g_matProj));*/
	// column_major + ��ġ�� ���
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
