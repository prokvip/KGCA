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
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
	float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD;
};

struct VS_INPUT
{
    float4 p : POSITION;
	float3 n : NORMAL;
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
	output.n = input.n;
    output.t  = input.t;
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
float4 COLOR_PS( VS_OUTPUT input ) : SV_Target
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
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, COLOR_PS() ) );

		SetBlendState( AlphaBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
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

		SetBlendState( AlphaBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		//SetRasterizerState( WireNoneCullSolidBais );
        //SetDepthStencilState( EnableDepthEqual, 0 );
    }
}
//--------------------------------------------------------------------------------------
technique11 DefaultRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

		SetBlendState( AlphaBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		//SetRasterizerState( WireNoneCullSolidBais );
       // SetDepthStencilState( EnableDepthEqual, 0 );
    }
}

