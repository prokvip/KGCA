//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D	g_txDiffuse : register (t0);
Texture2D	g_txNormalMap	: register (t1);
Texture2D	g_txHeightMap	: register (t2);
SamplerState g_samLinear: register (s0);

cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};

//-----------------------------------------------------------------------------
//상수 버퍼
//-----------------------------------------------------------------------------
cbuffer cbObjectChangesEveryFrame: register(b1)
{
	// Global
	matrix				g_matNormal;
	float3				cb_vLightVector;
	float3				cb_vEyePos;
	float3				cb_vEyeDir;
};

cbuffer cbObjectNeverChanges: register(b2)
{
	// Global	
	float4				cb_AmbientLightColor: packoffset(c0);
	float4				cb_DiffuseLightColor: packoffset(c1);
	float3				cb_SpecularLightColor: packoffset(c2);
	float				cb_SpecularPower : packoffset(c2.w);
};

struct PNCT2_VS_INPUT
{
	float4 p		: POSITION;
	float3 n		: NORMAL;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
	float3 Tan		: TANGENT;
};

struct PNCT4_PS_INPUT
{
	float4 p			: SV_POSITION;
	float3 n			: NORMAL;
	float4 c			: COLOR0;
	float2 t			: TEXCOORD0;
	float3 vEye			: TEXCOORD1;
	float3 vHalf		: TEXCOORD2;
	float3 vLightDir	: TEXCOORD3;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PNCT4_PS_INPUT VS(PNCT2_VS_INPUT input)
{
	PNCT4_PS_INPUT output = (PNCT4_PS_INPUT)0;
	float4 vWorldPos = mul(input.p, g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	output.p = mul(vViewPos, g_matProj);

	output.t = input.t*30.0f;
	output.c = input.c;
	
	// add
	float fNear = 0.1f;
	float fFar = 1000.0f;
	output.c.w = (output.p.w - fNear) / (fFar - fNear);

	float3 vNormal = normalize(mul(input.n, (float3x3)g_matNormal));
	output.n = vNormal;

	output.vEye = normalize(cb_vEyeDir);

	float3 T = normalize(mul(input.Tan, (float3x3)g_matNormal));
	float3 B = normalize(cross(vNormal, T));

	float3x3 tanMat = { T.x, B.x, vNormal.x,
		T.y, B.y, vNormal.y,
		T.z, B.z, vNormal.z };

	float3 vLightDir = cb_vLightVector.xyz;// normalize(cb_vLightVector.xyz - vWorldPos);
	output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), tanMat));
	output.vLightDir = normalize(mul(-vLightDir, tanMat));
	output.vEye = normalize(mul(output.vEye, tanMat));
	return output;
}

struct PBUFFER_OUTPUT
{
	float4 color0 : SV_TARGET0;
	float4 color1 : SV_TARGET1;
};

PBUFFER_OUTPUT PS(PNCT4_PS_INPUT input )
{ 
	PBUFFER_OUTPUT output;
	
	const float sfHeightBias = 0.01;
	float3 vViewTS   = normalize( input.vEye ); 
    float3 vLightTS  = normalize( -input.vLightDir );
	// 기본 텍스쳐 컬러 
	float4 DiffuseColor =	g_txDiffuse.Sample(g_samLinear, input.t );
	float4 HeightColor  =	g_txHeightMap.Sample(g_samLinear, input.t );
	float2 UV = input.t;

	// 디퓨즈 조명 
	float4 normal		=	g_txNormalMap.Sample(g_samLinear, UV ) * 2.0f -1.0f;
		  // normal		=	normalize( (normal - 0.5f) * 2.0f );			
	float  fDot			=	max (0, dot( normal.xyz, vLightTS ));
	float3 LightColor	=	cb_DiffuseLightColor * fDot;	

	// 스펙큘러 조명 	
	float3 V			=	input.vEye;	
	float3 half_vector	=	normalize( vLightTS + vViewTS );
	float3 R			=	reflect( vLightTS, normal.xyz); 	
	float3 SpecColor	=	cb_SpecularLightColor * pow( saturate(dot(R,V)), cb_SpecularPower );

	// 전체 컬러 조합  	
	float4 vFinalColor	=	input.c * DiffuseColor +  float4( LightColor, 1.0f );    

	//float diffuseToon = (fDot < 0.5) ? ((fDot < 0.25)? 0.47f : 0.71f ) : 1.0f;
	//float specularToon = (pow( saturate(dot(R,V)), cb_SpecularPower ) < 0.5) ? 0 : 1;
	//float4 co0lor = float4((diffuseToon + specularToon), (diffuseToon + specularToon)*0.8, 0, 1.0f);	
	output.color0 =  vFinalColor;
	float3 vNormal = input.n*0.5f+0.5f;		
	output.color1 = float4(vNormal,input.c.w);
	return output;
}
