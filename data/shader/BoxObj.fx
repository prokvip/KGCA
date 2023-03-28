matrix					g_matWorld;
matrix					g_matView;
matrix					g_matProj;
matrix					g_matNormal;
Texture2D				g_txDiffuse;

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
RasterizerState Solid
{
	CullMode = BACK;
	FillMode = SOLID;
};
SamplerState g_samWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
RasterizerState NoneCullSolid
{
    CullMode = NONE;
    FillMode = SOLID;
    DepthBias = 0.0f;
    SlopeScaledDepthBias = 0.0f;
};
RasterizerState WireNoneCullSolidBais
{
    CullMode = BACK;
    FillMode = WIREFRAME;
     DepthBias = 10.0f;
    SlopeScaledDepthBias = 1.0f;
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
DepthStencilState EnableDepthEqual
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ALL;
	DepthFunc		= LESS_EQUAL;
};
    
BlendState AlphaBlendingOne
{
	AlphaToCoverageEnable	= TRUE;
	BlendEnable[0]			= TRUE;
	SrcBlend				= ONE;
	DestBlend				= ZERO;
	BlendOp					= ADD;
	SrcBlendAlpha			= ONE;
	DestBlendAlpha			= ZERO;
	BlendOpAlpha			= ADD;
	RenderTargetWriteMask[0] = 0x0F;
};	

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT HEIGHTMAP_VS( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.p = mul( input.p, g_matWorld );
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );    
    output.t = input.t;
    output.c = input.c;
    
    return output;
}
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT DEBUG_MAP_VS( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.p = mul( input.p, g_matWorld );
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );    
    output.t = input.t;
    output.c = input.c;
    
    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 HEIGHTMAP_PS( VS_OUTPUT input) : SV_Target
{
    return g_txDiffuse.Sample( g_samWrap, input.t );// * input.c;    
}
float4 DEBUG_HEIGHTMAP_PS( VS_OUTPUT input) : SV_Target
{
    return float4( 1.0f, 0.0f, 1.0f, 1.0f );    
}
//--------------------------------------------------------------------------------------
technique11 DefaultRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, HEIGHTMAP_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, HEIGHTMAP_PS() ) );
     
		SetRasterizerState( Solid );   
        SetDepthStencilState( EnableDepth, 0 );
    }
}
technique11 DebugRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, HEIGHTMAP_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, DEBUG_HEIGHTMAP_PS() ) );
        
        SetRasterizerState( WireNoneCullSolidBais );
        SetDepthStencilState( EnableDepthEqual, 0 );
       // SetBlendState( AlphaBlendingOne, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}