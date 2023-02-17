
Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

cbuffer cb_data : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);	
	matrix g_matWorldInverse : packoffset(c12);

	float3  g_vLightDir : packoffset(c16);
	float  g_fRadius1 : packoffset(c16.w);

	float3  g_vLightPos : packoffset(c17);
	float  g_fRadius2 : packoffset(c17.w);

	float3  g_vEyeDir : packoffset(c18);
	float  g_fRadius3 : packoffset(c18.w);

	float3  g_vEyePos : packoffset(c19);
	float  g_fRadius4 : packoffset(c19.w);

	float  g_fValue1 : packoffset(c20.x);
	float  g_fValue2 : packoffset(c20.y);
	float  g_fValue3 : packoffset(c20.z);
	float  g_fTimer : packoffset(c20.w);
};