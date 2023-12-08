#define MAX_BONE_MATRICES 255 
struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;    // COLOR0 ~ COLOR1
    float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15

    float4 i : INDEX;
    float4 w : WEIGHT;
    float3 tan : TANGENT;
};
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};
// 상수버퍼(레지스터 단위로 저장되어야 한다.)
// 레지스터 단위란, float4(x,y,z,w) 
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4개    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};
cbuffer cbAnimMatrices : register (b1)
{
    float4x4 g_matBoneWorld[MAX_BONE_MATRICES];
};
//
//Buffer<float4>     g_BufferBoneWorld : register(t1);
//float4x4 FetchBoneTransform(uint iBone)
//{
//    float4x4 mret;
//    iBone *= 4;
//    float4 row1 = g_BufferBoneWorld.Load(iBone + 0);
//    float4 row2 = g_BufferBoneWorld.Load(iBone + 1);
//    float4 row3 = g_BufferBoneWorld.Load(iBone + 2);
//    float4 row4 = g_BufferBoneWorld.Load(iBone + 3);
//    mret = float4x4(row1, row2, row3, row4);
//    return mret;
//}
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;

    float4 vLocal = float4(vIn.p.xyz, 1.0f);
    float4 vWorld = 0;
    float3 vNormal = 0;

    for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
    }

    vWorld = mul(vWorld,g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vNormal;
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
    //            r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    return g_txDiffuse1.Sample(sample0, vIn.t) *vIn.c;
    //return vIn.c;
}