matrix					g_matWorld;
matrix					g_matView;
matrix					g_matProj;
matrix					g_matNormal;
Texture2D				g_txDiffuse;
float4					g_ParticleColor = { 1.0f, 1.0f,1.0f,1.0f};

SamplerState g_samWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState g_samWrap2
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Mirror;
	AddressV = Wrap;
};
//--------------------------------------------------------------------------------------
// DepthStates
//--------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ALL;
	DepthFunc		= LESS;
};
DepthStencilState DisableDepth
{
	DepthEnable		= FALSE;
	DepthWriteMask	= 0;
	DepthFunc		= LESS;
};	
//--------------------------------------------------------------------------------------
// 입출력 정점 구조
//--------------------------------------------------------------------------------------	
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t    : TEXCOORD0; 
};

struct VS_INPUT
{
	float3 p	: POSITION;
	float4 c	: COLOR0;
	float2 t	: TEXCOORD0;	  
};

#define MAX_BONE_MATRICES 255


cbuffer cbAnimMatrices
{
    matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

struct P3N3C4_VS_INPUT
{
    float4 p		: POSITION;
    float3 n		: NORMAL;
    float4 c		: COLOR0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT ASE_VS( P3N3C4_VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.p = mul( input.p, g_matWorld );
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );    
    output.c = g_ParticleColor;    
   
   //Bone0
  /*  uint iBone = Input.Bones.x;
    float fWeight = Input.Weights.x;
    matrix m = FetchBoneTransform( iBone, iFetchType );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );
    
    //Bone1
    iBone = Input.Bones.y;
    fWeight = Input.Weights.y;
    m = FetchBoneTransform( iBone, iFetchType );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );
    
    //Bone2
    iBone = Input.Bones.z;
    fWeight = Input.Weights.z;
    m = FetchBoneTransform( iBone, iFetchType );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );
    
    //Bone3
    iBone = Input.Bones.w;
    fWeight = Input.Weights.w;
    m = FetchBoneTransform( iBone, iFetchType );
    Output.Pos += fWeight * mul( Pos, m );
    Output.Norm += fWeight * mul( Norm, (float3x3)m );
    Output.Tan += fWeight * mul( Tan, (float3x3)m );
    */
    
    return output;
}

float4 NO_TEXTURE_PS( VS_OUTPUT input) : SV_Target
{
    return input.c * float4(1.0f, 0.0f, 0.0f,1.0f );    
}
//--------------------------------------------------------------------------------------
technique11 DefaultRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, ASE_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, NO_TEXTURE_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}
technique11 ColorRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, ASE_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, NO_TEXTURE_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}