//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
TextureCube g_txCubeMap: register (t0);
SamplerState g_samLinear: register (s0);
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float3 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float3 t : TEXCOORD0;	
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
	vOut.p			= vOut.p.xyww;

	vOut.c			= vIn.c;
	vOut.t			= normalize(vIn.p.xyz); 
	vOut.n			= normalize(mul(vIn.n, (float3x3)g_matWorld));	
    return vOut;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{ 	
	return g_txCubeMap.Sample(g_samLinear, vIn.t);
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
