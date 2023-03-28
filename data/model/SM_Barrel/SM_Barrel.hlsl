// Physically Based Rendering
// Copyright (c) 2017-2018 Michał Siejak

// Physically Based shading model: Lambetrtian diffuse BRDF + Cook-Torrance microfacet specular BRDF + IBL for ambient.

// This implementation is based on "Real Shading in Unreal Engine 4" SIGGRAPH 2013 course notes by Epic Games.
// See: http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

static const uint NumLights = 3;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;

//cbuffer TransformConstants : register(b0)
//{
//	float4x4 viewProjectionMatrix;
//	float4x4 skyProjectionMatrix;
//	float4x4 sceneRotationMatrix;
//};
//
//cbuffer ShadingConstants : register(b0)
//{
//	struct {
//		float3 direction;
//		float3 radiance;
//	} lights[NumLights];
//	float3 vEyePos;
//};
//
//struct VertexShaderInput
//{
//	float3 position  : POSITION;
//	float3 normal    : NORMAL;
//	float4 color     : COLOR;
//	float3 tangent   : TANGENT;
//	float3 bitangent : BITANGENT;
//	float2 texcoord  : TEXCOORD;
//};
//struct PixelShaderInput
//{
//	float4 pixelPosition : SV_POSITION;
//	float3 position : POSITION;
//	float2 texcoord : TEXCOORD;
//	float3x3 tangentBasis : TBASIS;
//};
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
	float4 p : SV_POSITION; // 3 -> 4
	float3 n : NORMAL;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
	float3 w  : TEXCOORD1;
	float3 l  : TEXCOORD2;
	float3 e  : TEXCOORD3;
	float4 shadow  : TEXCOORD4;
	float3x3 tangentBasis : TBASIS;
};

// 상수버퍼(단위:레지스터 단위(float4)로 할당되어야 한다.)
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
cbuffer cb1 : register(b1)
{	
	float4   vLightDir : packoffset(c0);
	float4   vLightPos : packoffset(c1);
	float4   vEyeDir   : packoffset(c2);
	float4   vEyePos   : packoffset(c3);
	matrix   g_matLight: packoffset(c4);
};
cbuffer cb2 : register(b2)
{
	float4x4 g_matBoneWorld[255]; //65535 / 4
}


Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D metalnessTexture : register(t2);
Texture2D roughnessTexture : register(t3);
Texture2D emissiveTexture : register(t4);
TextureCube specularTexture : register(t5);
TextureCube irradianceTexture : register(t6);
Texture2D specularBRDF_LUT : register(t7);

SamplerState defaultSampler : register(s0);
SamplerState spBRDF_Sampler : register(s1);

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels()
{
	uint width, height, levels;
	specularTexture.GetDimensions(0, width, height, levels);
	return levels;
}
//DownColor * (1-up) + UpColor = DownColor + up * (UpColor - DownColor)
float3 CalcAmbient(float3 normal, float3 color)
{
	const float3 AmbientUp = float3(1.0f, 1.0f, 1.0f);
	const float3 AmbientDown = float3(0.3f, 0.3f, 0.3f);
	const float3 AmbientRange = AmbientUp - AmbientDown;
	float up = normal.y * 0.5 + 0.5;
	float3 ambient = AmbientDown + up * AmbientRange;
	return ambient * color;
}
// Vertex shader
VS_OUTPUT VS(VS_INPUT v)
{
	VS_OUTPUT pOut;
	float4 vLocal = float4(v.p.xyz, 1.0f);// float4(v.p.x, v.p.y, v.p.z, 1.0f);
	//float4 vWorld = mul(vLocal, g_matBoneWorld[iIndex]);
	float4 vWorld = 0;
	float3 vNormal = 0;
	for (int iBone = 0; iBone < 4; iBone++)
	{
		uint iBoneIndex = v.i[iBone];
		vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * v.w[iBone];
		vNormal += mul(v.n, (float3x3)g_matBoneWorld[iBoneIndex]) * v.w[iBone];
	}

	vWorld = mul(vWorld, g_matWorld);
	vNormal = mul(vNormal, (float3x3)g_matNormal);
	pOut.shadow = mul(vWorld, g_matLight);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	pOut.p = vProj;
	pOut.n = normalize(vNormal);
	pOut.t = v.t;
	pOut.c = v.c;

	float3 T = normalize(mul(v.tan, (float3x3)g_matNormal));
	float3 B = normalize(cross(vNormal, T));
	 T = normalize(cross(B, vNormal));

	//float3x3 tanMat = { T.x, T.y, T.z,
	//					B.x, B.y, B.z,
	//					vNormal.x, vNormal.y, vNormal.z };
	//pOut.l = normalize(mul(-vLightDir.xyz, tanMat));
	//pOut.e = normalize(mul(vEyeDir.xyz, tanMat));
	pOut.w = vWorld.xyz;

	float3x3 TBN = float3x3(T, B, vNormal);
	pOut.tangentBasis = transpose(TBN);
	return pOut;
}

// Pixel shader
float4 PS(VS_OUTPUT pin) : SV_Target
{
	float3 albedo = albedoTexture.Sample(defaultSampler, pin.t).rgb;
	float metalness = metalnessTexture.Sample(defaultSampler, pin.t).g;
	float roughness = roughnessTexture.Sample(defaultSampler, pin.t).b;
	float3 emisive = emissiveTexture.Sample(defaultSampler, pin.t);

	float3 Lo = normalize(vEyePos.xyz - pin.w);
	float3 N = normalize(2.0 * normalTexture.Sample(defaultSampler, pin.t).rgb - 1.0);
	N = normalize(mul(pin.tangentBasis, N));
	float cosLo = max(0.0, dot(N, Lo));
	float3 Lr = 2.0 * cosLo * N - Lo;
	float3 F0 = lerp(Fdielectric, albedo, metalness);
	
	float3 directLighting = 0.0;
	for (uint i = 0; i < 1; ++i)
	{
		float3 Li = -vLightDir.xyz;
		float3 Lradiance = float3(1,1,1);
		float3 Lh = normalize(Li + Lo);
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));
		float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		float D = ndfGGX(cosLh, roughness);
		float G = gaSchlickGGX(cosLi, cosLo, roughness);
		float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
		float3 diffuseBRDF = kd * albedo;
		float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);
		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	}

	float3 ambient = CalcAmbient(pin.n, albedo);
	float3 ambientLighting;
	{		
		float3 irradiance = irradianceTexture.Sample(defaultSampler, N).rgb;
		float3 F = fresnelSchlick(F0, cosLo);
		float3 kd = lerp(1.0 - F, 0.0, metalness);
		float3 diffuseIBL = kd * albedo * irradiance;

		uint specularTextureLevels = querySpecularTextureLevels();
		float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;
		float2 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosLo, roughness)).rg;
		float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
		ambientLighting = diffuseIBL + specularIBL;
	}
	//return float4(ambient + emisive+ directLighting, 1.0);
	return float4(ambient + emisive + directLighting+ ambientLighting, 1.0);
}
