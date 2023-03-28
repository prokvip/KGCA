matrix					g_matWorld;
matrix					g_matView;
matrix					g_matProj;
matrix					g_matNormal;
float4					g_vAmbientColor = { 1.0f, 1.0f, 1.0f, 1.0f };

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
struct PNCT2_VS_INPUT
{
    float4 p		: POSITION;
    float3 n		: NORMAL;
    float4 c		: COLOR0;
    float2 t		: TEXCOORD0;
};

SamplerState g_samWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
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
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT HEIGHTMAP_VS( PNCT2_VS_INPUT input )
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
VS_OUTPUT DEBUG_MAP_VS( PNCT2_VS_INPUT input )
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
    return g_txDiffuse.Sample( g_samWrap, input.t ) * g_vAmbientColor;    
}
//--------------------------------------------------------------------------------------
technique11 DefaultRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, HEIGHTMAP_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, HEIGHTMAP_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}
technique11 DebugRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, DEBUG_MAP_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, HEIGHTMAP_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}