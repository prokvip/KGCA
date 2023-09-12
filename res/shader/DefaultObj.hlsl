struct VS_INPUT
{
    float3 p : POSITION;
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
// 상수버퍼(레지스터 단위로 저장되어야 한다.)
// 레지스터 단위란, float4(x,y,z,w) 
cbuffer cb0 
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4개    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // v * m ->(행->c0,c1,c2,c3)
    // v dot c0 = v.x
    // v dot c1 = v.y
    // v dot c2 = v.z
    // v dot c3 = v.w     
    float4 vWorld = mul(float4(vIn.p,1.0f),g_matWorld);
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
    //            r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    return g_txDiffuse1.Sample(sample0, vIn.t) * vIn.c;
}