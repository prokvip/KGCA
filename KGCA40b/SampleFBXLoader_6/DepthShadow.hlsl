cbuffer cb0 : register(b0)
{
	// 1개의 레지스터(x,y,z,w)
	matrix   g_matWorld : packoffset(c0);
	matrix   g_matView : packoffset(c4);
	matrix   g_matProj : packoffset(c8);
	matrix	 g_matNormal : packoffset(c12);
	float4   Color0 : packoffset(c16);	
	float    TimerX : packoffset(c17.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
struct VS_OUTPUT
{
    float4 p  : SV_POSITION; 
	float4 c  : COLOR0;
};
//--------------------------------------------------------------------------------------
// Composite partices into the scene
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 p : POSITION )
{
    VS_OUTPUT output;
	output.p = mul(p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	float fNear = 0.1f;
	float fFar = 5000.0f;
	output.c = (output.p.w - fNear) / (fFar - fNear);
    return output;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
	return vIn.c;
}