#define MAX_BONE_MATRICES 255 
struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;    // COLOR0 ~ COLOR1
    float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
};
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};
// �������(�������� ������ ����Ǿ�� �Ѵ�.)
// �������� ������, float4(x,y,z,w) 
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4��    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    float4 vLocal = float4(vIn.p.xyz, 1.0f);
    float4 vWorld = mul(vLocal,g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    return vOut;
}

Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);
struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    //            r,g,b,a(1)=������, a(0)=��������, a(0.0< 1.0f)= ������
    return g_txDiffuse1.Sample(sample0, vIn.t) *vIn.c;
    //return vIn.c;
}