//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//cbuffer cb1
//{
	matrix g_matWorld;;
	matrix g_matView;
	matrix g_matProj;
	
//};

float4 g_vAmbientColor = { 1.0f, 1.0f, 1.0f, 1.0f };

BlendState AlphaBlending
{
	AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};
BlendState AddBlending
{
	AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

Texture2D g_txDiffuse;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
RasterizerState WireNoneCullSolidBais
{
    CullMode = BACK;
    FillMode = WIREFRAME;
    DepthBias = -1.0f;
    SlopeScaledDepthBias = 0.0f;
};
DepthStencilState EnableDepthEqual
{
	DepthEnable		= TRUE;
	DepthWriteMask	= ALL;
	DepthFunc		= LESS_EQUAL;
};
//--------------------------------------------------------------------------------------
struct NO_TEX_VS_OUTPUT
{
    float4 p : SV_POSITION;
    float4 c : COLOR0;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float4 c : COLOR0;
    float2 t : TEXCOORD;
};

struct VS_INPUT
{
    float4 p : POSITION;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(	VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.p = mul( input.p, g_matWorld );   
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );
    output.c = input.c;
    output.t  = input.t;
    return output;
}
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

NO_TEX_VS_OUTPUT COLOR_VS( float4 p : POSITION, float4 c : COLOR )
{
    NO_TEX_VS_OUTPUT output = (NO_TEX_VS_OUTPUT)0;
    output.p = mul( p, g_matWorld );   
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );
    output.c = c;
    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return g_txDiffuse.Sample( samLinear, input.t ) * input.c * g_vAmbientColor;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 COLOR_PS( NO_TEX_VS_OUTPUT input ) : SV_Target
{
    return input.c * g_vAmbientColor;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 AmbientColor( VS_OUTPUT input ) : SV_Target
{
   return  g_vAmbientColor;
}
//--------------------------------------------------------------------------------------
technique11 ColorRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, COLOR_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, COLOR_PS() ) );

		//SetBlendState( AddBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}
//--------------------------------------------------------------------------------------
technique11 AmbientColorRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, AmbientColor() ) );

		//SetBlendState( AddBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		//SetRasterizerState( WireNoneCullSolidBais );
        //SetDepthStencilState( EnableDepthEqual, 0 );
    }
}
//--------------------------------------------------------------------------------------
technique11 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

		//SetBlendState( AlphaBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		//SetRasterizerState( WireNoneCullSolidBais );
       // SetDepthStencilState( EnableDepthEqual, 0 );
    }
}

