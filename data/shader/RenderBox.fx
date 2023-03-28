//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix g_matWorld;
matrix g_matView;
matrix g_matProj;


//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float4 c : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 p : POSITION, float4 c : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
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
    return input.c;
}


//--------------------------------------------------------------------------------------
technique11 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

