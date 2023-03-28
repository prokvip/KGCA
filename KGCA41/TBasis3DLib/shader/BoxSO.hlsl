//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse: register (t0);
SamplerState g_samLinear: register (s0);
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb1: register(b1)
{
	matrix				g_matWorldInverse: packoffset(c0);
	float4				g_cAmbientMaterial: packoffset(c4);
	float4				g_cDiffuseMaterial: packoffset(c5);
	float4				g_cSpecularMaterial: packoffset(c6);
	float4				g_cEmissionMaterial: packoffset(c7);

	float4				g_cAmbientLightColor : packoffset(c8);
	float4				g_cDiffuseLightColor: packoffset(c9);
	float4				g_cSpecularLightColor: packoffset(c10);
	float3				g_vLightDir : packoffset(c11);	
	float			    g_fDamping : packoffset(c11.w);
	float3				g_vLightPos : packoffset(c12);
	float			    g_fRadius : packoffset(c12.w);
	float3				g_vEyeDir : packoffset(c13);
	float			    g_fIntensity : packoffset(c13.w);
	float3				g_vEyePos : packoffset(c14);
	float			    g_fEyeRadius : packoffset(c14.w);
};
//--------------------------------------------------------------------------------------
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
	//float4 d : TEXCOORD1;
};

struct S0_VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;	
};
struct SO_VS_OUTPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
};
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p			= mul( float4(vIn.p,1.0f), g_matWorld );
	vOut.p			= mul(vOut.p, g_matView );
	vOut.p			= mul(vOut.p, g_matProj );
	vOut.t			= vIn.t;
	vOut.c			= vIn.c;
    //matrix	matTransform= transpose( g_matWorldInverse );    
	vOut.n = normalize(mul(vIn.n, (float3x3)g_matWorldInverse));
    return vOut;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 Diffuse(float3 vNormal)
{
	float fIntensity = max(0, dot(vNormal, normalize(-g_vLightDir)));
	float4 diffuse = g_cAmbientMaterial * g_cAmbientLightColor +
		(g_cDiffuseMaterial * g_cDiffuseLightColor * fIntensity);
	return diffuse;
}
float4 PS(VS_OUTPUT vIn) : SV_Target
{ 
	float4 vTexColor = g_txDiffuse.Sample( g_samLinear, vIn.t );
	float4 vFinalColor = vTexColor * Diffuse( vIn.n) * vIn.c;
	vFinalColor.a = 1.0f;
	return vFinalColor;
}

float4 PS_Texture(VS_OUTPUT vIn) : SV_Target
{
	return g_txDiffuse.Sample(g_samLinear, vIn.t);
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{	
	float4 vColor = Diffuse( vIn.n );
	vColor.a = 1.0f;
	return vColor;
}
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SO_VS_OUTPUT VS_SO(S0_VS_INPUT input )
{
	SO_VS_OUTPUT output = (SO_VS_OUTPUT)0;
    output.p = input.p;
    output.n = input.n;
    output.c = input.c;    
    output.t = input.t;    
    return output;
}
// 로칼 정점의 크기를 정규화하여 1로 만든다.
SO_VS_OUTPUT NormalizeVertex(SO_VS_OUTPUT Vertex )
{
	SO_VS_OUTPUT newvertex;
	newvertex.p = normalize( Vertex.p.xyz );
	// 정점이 원점을 중심으로 하는 노말 벡터가 된다.
	newvertex.n = normalize( Vertex.p.xyz ); 
	newvertex.c = Vertex.c; 
	newvertex.t = Vertex.t;    
	return newvertex;
}
void TriAppend(SO_VS_OUTPUT V0, SO_VS_OUTPUT V1, SO_VS_OUTPUT V2, inout TriangleStream<SO_VS_OUTPUT> TriStream )
{
	TriStream.Append( NormalizeVertex(V0));
	TriStream.Append( NormalizeVertex(V1));	
	TriStream.Append( NormalizeVertex(V2));	
	// 삼각형 단위로 스트립으로 구성토록 한다.		
	TriStream.RestartStrip();
}
[maxvertexcount(12)]
void GS_SO(triangle SO_VS_OUTPUT input[3], inout TriangleStream<SO_VS_OUTPUT> TriStream)
{	
	// 페이스의 중점을 구한다.
	SO_VS_OUTPUT Center0, Center1, Center2;
    Center0.p = (input[0].p.xyz + input[1].p.xyz)/2.0;
    Center0.n = (input[0].n.xyz + input[1].n.xyz)/2.0;
    Center0.c = (input[0].c + input[1].c)/2.0;  
    Center0.t = (input[0].t.xy + input[1].t.xy)/2.0;      

	Center1.p = (input[1].p.xyz + input[2].p.xyz)/2.0;
    Center1.n = (input[1].n.xyz + input[2].n.xyz)/2.0;
    Center1.c = (input[1].c + input[2].c)/2.0;  
    Center1.t = (input[1].t.xy + input[2].t.xy)/2.0;      

	Center2.p = (input[0].p.xyz + input[2].p.xyz)/2.0;
    Center2.n = (input[0].n.xyz + input[2].n.xyz)/2.0;
    Center2.c = (input[0].c + input[2].c)/2.0;  
    Center2.t = (input[0].t.xy + input[2].t.xy)/2.0;      

    TriAppend( input[0], Center0,Center2,  TriStream );				
	TriAppend( Center0, input[1], Center1, TriStream );		
	TriAppend( input[2], Center2, Center1, TriStream );				
	TriAppend( Center0, Center1, Center2, TriStream );		
}
