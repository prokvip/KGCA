cbuffer cb0 : register(b0)
{
	// 1개의 레지스터(x,y,z,w)
	matrix   g_matWorld : packoffset(c0);
	matrix   g_matView : packoffset(c4);
	matrix   g_matProj : packoffset(c8);
	float4   Color0 : packoffset(c12);
	float    TimerX : packoffset(c13.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
struct VS_OUTPUT
{
    float4 p  : SV_POSITION; 
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
    return output;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
	return float4(1,1,1,1);
}