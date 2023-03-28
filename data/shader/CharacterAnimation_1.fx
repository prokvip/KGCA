matrix					g_matWorld;
matrix					g_matView;
matrix					g_matProj;
matrix					g_matNormal;
matrix					g_matWorldInvTrans;
Buffer<float4>			g_BufferBoneWorld;
Texture2D				g_txDiffuse;


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

struct P3N3C4T5_VS_INPUT
{
    float3 p		: POSITION;
    float3 n		: NORMAL;
    float4 c		: COLOR0;
    float2 t		: TEXCOORD0;    
    float4 w0		: TEXCOORD1;
    float4 i0		: TEXCOORD2;
    float4 w1		: TEXCOORD3;
    float4 i1		: TEXCOORD4;
};

struct SKIN_VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0; 
};
float4x4 FetchBoneTransform( uint iBone )
{
    float4x4 mret;
    iBone *= 4;
    float4 row1 = g_BufferBoneWorld.Load( iBone + 0 );
    float4 row2 = g_BufferBoneWorld.Load( iBone + 1 );
    float4 row3 = g_BufferBoneWorld.Load( iBone + 2 );
    float4 row4 = g_BufferBoneWorld.Load( iBone + 3 );
    mret = float4x4( row1, row2, row3, row4 );
    return mret;
}
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SKIN_VS_OUTPUT Character_VS( P3N3C4T5_VS_INPUT input  )//,uniform bool bHalfVector )
{
    SKIN_VS_OUTPUT output = (SKIN_VS_OUTPUT)0;
   
    float4 Pos = float4(input.p,1);
    float3 Norm =input.n;
    
    float4x4 matMatrix;
	
	float4 vLocal;
	for( int iBiped = 0 ; iBiped < input.w1.w; iBiped++ )
	{
		uint iBoneIndex = (uint)input.i0[iBiped];
		float fWeight	= input.w0[iBiped];
	
		vLocal = Pos;
		if( iBiped < 4 ) 
		{
			matMatrix	= FetchBoneTransform( iBoneIndex );
			output.p	+= fWeight * mul( vLocal, matMatrix );						    
			output.n	+= fWeight * mul( Norm, (float3x3)matMatrix );
		}
		else
		{
			iBoneIndex= (uint)input.i1[iBiped];
			fWeight		= input.w1[iBiped];
			matMatrix	= FetchBoneTransform( iBoneIndex );	
			output.p	+= fWeight * mul( vLocal, matMatrix );		
				    
			output.n	+= fWeight * mul( Norm, (float3x3)matMatrix );
		}
	}	
	
	output.p = mul( output.p, g_matWorld );	
	output.n = normalize(mul( Norm, (float3x3)g_matWorldInvTrans ));
	
    output.p = mul( output.p, g_matView );
    output.p = mul( output.p, g_matProj );    
    output.t = input.t;
    output.c = input.c;    
    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 Character_PS( SKIN_VS_OUTPUT input) : SV_Target
{
    return g_txDiffuse.Sample( g_samWrap, input.t ) * input.c;    
}
float4 NO_TEXTURE_PS( SKIN_VS_OUTPUT input) : SV_Target
{
    return input.c * float4(1.0f, 0.0f, 0.0f,1.0f );    
}
//--------------------------------------------------------------------------------------
technique11 DefaultRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, Character_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, Character_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}
technique11 ColorRender
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, Character_VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, NO_TEXTURE_PS() ) );
        
        SetDepthStencilState( EnableDepth, 0 );
    }
}