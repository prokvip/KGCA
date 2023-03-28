#define MAX_BONE_MATRICES 255 
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
Texture2D g_txDiffuseLayer1: register (t1);
Texture2D g_txDiffuseLayer2: register (t2);
SamplerState samLinear: register (s0);
cbuffer cb0
{
	matrix 	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix 	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
cbuffer cbAnimMatrices : register (b1)
{
	float4x4 m_matConstBoneWorld[MAX_BONE_MATRICES];
};

Buffer<float4>     g_BufferBoneWorld : register(t1);
float4x4 FetchBoneTransform(uint iBone)
{
	float4x4 mret;
	iBone *= 4;
	float4 row1 = g_BufferBoneWorld.Load(iBone + 0);
	float4 row2 = g_BufferBoneWorld.Load(iBone + 1);
	float4 row3 = g_BufferBoneWorld.Load(iBone + 2);
	float4 row4 = g_BufferBoneWorld.Load(iBone + 3);
	mret = float4x4(row1, row2, row3, row4);
	return mret;
}
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD0;
	float4 i : INDEX;
	float4 w : WEIGHT;
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
	float4 Pos = float4(vIn.p, 1);
	float3 vNorm;
	float4x4 matMatrix;
	float4 vLocal = Pos;
	for (int iBiped = 0; iBiped < 4; iBiped++)
	{
		uint iBoneIndex = (uint)vIn.i[iBiped];
		float fWeight = vIn.w[iBiped];
		matMatrix = FetchBoneTransform(iBoneIndex); //m_matConstBoneWorld[iBoneIndex];
													//matMatrix = m_matConstBoneWorld[iBoneIndex];
		output.p += fWeight * mul(vLocal, matMatrix);
		vNorm += fWeight * mul(vIn.n, (float3x3)matMatrix);
	}

	float4 vWorld = mul(output.p, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	output.p = mul(vView, g_matProj);

	output.n = normalize(mul(vNorm, (float3x3)g_matWorld));
	output.t = vIn.t;
	output.c = vIn.c;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float3 vLight = float3(g_MeshColor.x,g_MeshColor.y,g_MeshColor.z);
	vLight = normalize(vLight);
	float fDot = max(0.4f, dot(-vLight, vIn.n));

	float4 vColor = g_txDiffuse.Sample(samLinear, vIn.t);
	float2 vLayer1 = vIn.c.xy;
	float2 vLayer2 = vIn.c.zw;
	float4 vColorLayer1 = g_txDiffuseLayer1.Sample(samLinear, vIn.t);
	float4 vColorLayer2 = g_txDiffuseLayer2.Sample(samLinear, vIn.t);
	float4 vTextureColor = (vColor* vColorLayer1*vColorLayer2);
	if (vTextureColor.a <= 0.001f)
	{
		discard;
	}
	return  vTextureColor *float4(fDot, fDot, fDot, 1);// *vIn.c;	
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Texture(VS_OUTPUT vIn) : SV_Target
{
	return g_txDiffuse.Sample(samLinear, vIn.t);
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
	return vIn.c;
}