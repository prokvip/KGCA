struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 3 -> 4
	float3 n : NORMAL;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15	
	float3 r : TEXCOORD1;
};
struct PS_INPUT
{
	float4 p : SV_POSITION; // 3 -> 4
	float3 n : NORMAL;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
	float3 r : TEXCOORD1; // TEXCOORD0 ~ TEXCOORD15
	uint   id : SV_PrimitiveID;
};
// �������(����:�������� ����(float4)�� �Ҵ�Ǿ�� �Ѵ�.)
cbuffer cb0 : register(b0)
{
	// 1���� ��������(x,y,z,w)
	matrix   g_matWorld : packoffset(c0);
	matrix   g_matView : packoffset(c4);
	matrix   g_matProj : packoffset(c8);
	float4   Color0 : packoffset(c12);
	float    TimerX : packoffset(c13.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
// �ý��۰� : �����ε���, ���̽��ε��� ��
VS_OUTPUT VS( VS_INPUT v)
{
	VS_OUTPUT pOut = (VS_OUTPUT)0;
	float4 vLocal = float4(v.p.xyz, 1.0f);// float4(v.p.x, v.p.y, v.p.z, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	pOut.p = vProj.xyzz;	
	pOut.n = v.n;
	pOut.t = v.t;	
	pOut.c = v.c;
	pOut.r = normalize(vLocal.xyz);
	return pOut;
}
Texture2D		g_txColor : register(t0);
Texture2D		g_txMask : register(t1);
Texture2D		g_txColorArray[6] : register(t2);
TextureCube	    g_txCubeMap : register(t3);

SamplerState	g_SampleLinear : register(s0);
SamplerState	g_SamplePoint : register(s1);


float4 GetTextureColor(uint index, float2 uv)
{
	float4 fColor = float4(1, 1, 1, 1);
	switch (index)
	{
	case 0: fColor = g_txColorArray[0].Sample(g_SamplePoint, uv); break;
	case 1: fColor = g_txColorArray[1].Sample(g_SamplePoint, uv); break;
	case 2: fColor = g_txColorArray[2].Sample(g_SamplePoint, uv); break;
	case 3: fColor = g_txColorArray[3].Sample(g_SamplePoint, uv); break;
	case 4: fColor = g_txColorArray[4].Sample(g_SamplePoint, uv); break;
	case 5: fColor = g_txColorArray[5].Sample(g_SamplePoint, uv); break;
	}
	return fColor;
}
//3��
float4 PS(PS_INPUT input) : SV_TARGET
{	
	float4 color = g_txCubeMap.Sample(g_SampleLinear, input.r);
	return color;
}

//2��
//float4 PS(PS_INPUT input) : SV_TARGET
//{
//	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
//	uint x = input.id / 2;	
//	float4 color = GetTextureColor(x, input.t);
//	return color;
//}
//1��
//float4 PS(VS_OUTPUT input) : SV_TARGET
//{
//	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
//	float4 color = g_txColor.Sample(g_SamplePoint, input.t);
//	return color;
//}
