struct VS_INPUT
{
	float3 p : POSITION;	   
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;	 // Ω√∏‡∆Ω(¿«πÃ±∏¡∂)
	float4 c : COLOR0;
    float2 t : TEXCOORD0;	
};

VS_OUTPUT VS( VS_INPUT vIn )
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1.0f);
	vOut.t = vIn.t;
	vOut.c = vIn.c ;
	return vOut;
}
VS_OUTPUT VS2( VS_INPUT vIn )
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1.0f);
	vOut.t = vIn.t;
	vOut.c = vIn.c * float4(1,1,0,1);
	return vOut;
}
VS_OUTPUT VS3( VS_INPUT vIn )
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p = float4(vIn.p.x, vIn.p.y, vIn.p.z, 1.0f);
	vOut.t = vIn.t;
	vOut.c = vIn.c* float4(1,0,0,1);
	return vOut;
}