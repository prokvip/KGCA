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
VS_OUT VS_KGCA(VS_IN vIn)
{
    VS_OUT vOut = (VS_OUT)0;
    vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1);
    vOut.t = vIn.t;
    return vOut;
}
