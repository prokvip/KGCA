
static const float gamma = 2.2;
static const float exposure = 1.0;
static const float pureWhite = 1.0;

struct VS_IN
{
	float3 p : POSITION; 
    float4 c : COLOR;
	float2 t : TEXTURE;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
VS_OUT VS(VS_IN input) 
{
	VS_OUT output = (VS_OUT)0;
	output.p = float4(input.p,1.0f);
	output.c = input.c;
	output.t = input.t;
	return output;
}

Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

float4 PS(VS_OUT input) : SV_Target
{
	float4 color = g_txTex.Sample(g_SampleWrap , input.t) * exposure;

	// Reinhard tonemapping operator.
	// see: "Photographic Tone Reproduction for Digital Images", eq. 4
	float luminance = dot(color, float3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

	// Scale color by ratio of average luminances.
	float3 mappedColor = (mappedLuminance / luminance) * color;

	// Gamma correction.
	return float4(pow(mappedColor, 1.0 / gamma), 1.0);	
}