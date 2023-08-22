struct VS_IN
{
    float3 p : POS;
    float2 t : TEXTURE;
};

struct VS_OUT
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

// �������(�������� ������ ����Ǿ�� �Ѵ�.)
// �������� ������, float4(x,y,z,w) 
cbuffer cb0 
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4��    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};
VS_OUT VS(VS_IN vIn)
{
    VS_OUT vOut = (VS_OUT)0;   
    // v * m ->(��->c0,c1,c2,c3)
    // v dot c0 = v.x
    // v dot c1 = v.y
    // v dot c2 = v.z
    // v dot c3 = v.w     
    float4 vWorld = mul(float4(vIn.p,1.0f),g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    //vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1);
    vOut.t = vIn.t;
    return vOut;
}

Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);
struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(PS_IN vIn) : SV_Target
{
    //            r,g,b,a(1)=������, a(0)=��������, a(0.0< 1.0f)= ������
    return g_txDiffuse1.Sample(sample0, vIn.t);
}