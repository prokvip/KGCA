#define MAX_MASKCOUNT 625

Texture2D g_txColor: register (t0);     
Texture2D g_txNormalDepth: register (t1); 

SamplerState g_samLinear: register (s0);

cbuffer cb1 : register(b1)
{
	float		iMask :  packoffset(c0.x);
	float		g_iTexSizeX : packoffset(c0.y);
	float		g_iTexSizeY : packoffset(c0.z);
};

Buffer<float>     g_fMaskArray : register(t2);

struct VS_OUTPUT
{
    float4 p   : SV_POSITION;   
	float2 t  : TEXCOORD; 
};
//--------------------------------------------------------------------------------------
// Composite partices into the scene
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 p : POSITION )
{
    VS_OUTPUT Output;
    Output.p = p;
    Output.t.x = (p.x + 1.0f) / 2.0f;	// x값은 -1 ~ 1의 값을 0~1로 변환 
	Output.t.y = (-p.y+ 1.0f) / 2.0f;	// y값은 1 ~ -1의 값을 0~1로 변환
    return Output;    
}
float4 GaussianBlur( float3 vTex, Texture2D tex2D )
{
	int		g_iMask = (int)iMask;
	int		g_iTexSizeX = 800;
	int		g_iTexSizeY = 600;
	float dx = 1.0f/g_iTexSizeX;
	float dy = 1.0f/g_iTexSizeY;
	float2 vStart = float2(	vTex.x-dx*g_iMask, 
							vTex.y-dy*g_iMask);
	float4 vSum = 0;
	float fX, fY;
	//[roll]
	for( int ix=0; ix < g_iMask; ix++ )
	{
		fX = vStart.x+dx*ix;
		//[roll]
		for( int iy=0; iy < g_iMask; iy++ )
		{							
			fY = vStart.y+dy*iy;
			vSum += tex2D.Sample(g_samLinear,float2(fX, fY))*g_fMaskArray[ix*g_iMask+iy];		
		}		
	}
	vSum.w = 1.0f;
	return vSum;
}
float4 Blur( float3 vTex, Texture2D tex2D)
{
	int		g_iMask = (int)iMask;
	int		g_iTexSizeX = 800;
	int		g_iTexSizeY = 600;
	float dx = 1.0f/g_iTexSizeX;
	float dy = 1.0f/g_iTexSizeY;
	float2 vStart = float2(	vTex.x-dx*g_iMask, 
							vTex.y-dy*g_iMask);
	float4 vSum = 0;
	float fX, fY;
	float fWeight = 1.0f/(g_iMask*g_iMask);
		
	//[roll]
	for( int ix=0; ix < g_iMask; ix++ )
	{
		fX = vStart.x+dx*ix;
		//[roll]
		for( int iy=0; iy < g_iMask; iy++ )
		{							
			fY = vStart.y+dy*iy;
			vSum += tex2D.Sample(g_samLinear,float2(fX, fY))*fWeight;		
		}		
	}
	vSum.w = 1.0f;
	return vSum;
}
float4 Sharp( float3 vTex, Texture2D tex2D )
{
	int		g_iMask = 3;
	int		g_iTexSizeX = 800;
	int		g_iTexSizeY = 600;

	float dx = 1.0f/g_iTexSizeX;
	float dy = 1.0f/g_iTexSizeY;
	float2 vStart = float2(	vTex.x-dx*g_iMask, 
							vTex.y-dy*g_iMask);
	float4 vSum = 0;
	float fX, fY;
	float fWeight=-1.0f;	
	int   iSharp = g_iMask * g_iMask/2;
	//[roll]
	for( int ix=0; ix < g_iMask; ix++ )
	{
		fX = vStart.x+dx*ix;
		//[roll]
		for( int iy=0; iy < g_iMask; iy++ )
		{	
			fWeight = -1.0f;
			if( ix* g_iMask+ iy == iSharp)
			{
				fWeight = g_iMask * g_iMask;
			}
			fY = vStart.y+dy*iy;
			vSum += tex2D.Sample(g_samLinear,float2(fX, fY))*fWeight;		
		}		
	}
	vSum.w = 1.0f;
	return vSum;
}
float4 Sobel( float3 vTex, Texture2D tex2D )
{
	float4 diffuseColor = tex2D.Sample(g_samLinear,vTex.xy);
	int		g_iTexSizeX = 800;
	int		g_iTexSizeY = 600;
	// -1 0 1
	// -2 0 2
	// -1 0 1
	float dx = 1.0f/g_iTexSizeX;
	float dy = 1.0f/g_iTexSizeY;

	float4 tl = tex2D.Sample(g_samLinear, float2(vTex.x-dx, vTex.y-dy));
	float4 l = tex2D.Sample(g_samLinear, float2(vTex.x-dx, vTex.y));
	float4 bl = tex2D.Sample(g_samLinear, float2(vTex.x-dx, vTex.y+dy));
	float4 t = tex2D.Sample(g_samLinear, float2(vTex.x, vTex.y-dy));
	float4 b = tex2D.Sample(g_samLinear, float2(vTex.x, vTex.y+dy));
	float4 tr = tex2D.Sample(g_samLinear, float2(vTex.x+dx, vTex.y-dy));
	float4 r = tex2D.Sample(g_samLinear, float2(vTex.x+dx, vTex.y));
	float4 br = tex2D.Sample(g_samLinear, float2(vTex.x+dx, vTex.y+dy));
	
	float4 SobelX = -tl - 2.0f *l - bl + tr + 2.0f *r + br;

	// -1 -2 -1
	// 0 0 0
	// 1 2 1
	float4 SobelY = -tl - 2.0f *t - tr + bl + 2.0f *b + br;
	float4 SobelResult = abs(SobelX) + abs(SobelY);
	float deltaNormal = 1.0f- 
		min(1.0f, 32.0f * pow((SobelResult.x + SobelResult.y + SobelResult.z ) / 3, 2.0f));
	float fDepth = 1.0f - 
		min(1.0f, 32.0f * pow(tl.a - br.a, 2.0f) + pow(bl.a - tr.a, 2.0f));
	deltaNormal = deltaNormal*fDepth;
	return float4(deltaNormal, deltaNormal, deltaNormal,1);
}

// 한 방향만의 마스크 생성
float4 GaussianPixelShader(float3 vTex, Texture2D tex2D) 
{
	//int		g_iTexSizeX = 800;
	//int		g_iTexSizeY = 600;

	const float Pixels[13] =
	{
		-6,    -5,   -4,   -3,   -2,   -1,    0,    1,    2,    3,    4,    5,    6,
	};

	const float BlurWeights[13] =
	{
		0.002216,   0.008764,   0.026995,   0.064759,   0.120985,   0.176033,
		0.199471,
		0.176033,   0.120985,   0.064759,   0.026995,   0.008764,   0.002216,
	};

    // Pixel width
    float dx = 1.0f/g_iTexSizeX;
	float dy = 1.0f/g_iTexSizeY;
    float4 color = {0, 0, 0, 1};

    float2 blur;
    blur.y = vTex.y;

    for (int i = 0; i < 13; i++) 
    {
        blur.x = vTex.x + Pixels[i] * dx;
        color += tex2D.Sample(g_samLinear, blur.xy) * BlurWeights[i];
    }  
    return color;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 vTexture = g_txColor.Sample(g_samLinear,input.t);
//	float4 vNormalColor = g_txNormalDepth.Sample(g_samLinear,input.t);	
//	// 스카이오브젝트 랜더링에 사용
//	if (vTexture.w < 0.01f)
//	{
//		discard;
//	}
////	// 내부 경계 및 외부 경계  	
//	float4 vEdgeColor =  Sobel(float3(input.t, 0.0f), g_txNormalDepth);
//	float4 vFinal = vEdgeColor *vTexture;
//#ifdef GAUSSIAN_BLUR
//	float4 vBlur = GaussianBlur(float3(input.t, 0.0f), g_txColor);
//#else
//	float4 vBlur = Blur(float3(input.t, 0.0f), g_txColor);
//#endif
//	float4 vSharp = Sharp(float3(input.t, 0.0f), g_txColor);	
//	float4 cSharp_Blur = lerp(  vBlur, vSharp,vNormalColor.w );
//	vFinal = lerp(cSharp_Blur, vFinal, vNormalColor.w );
//	vFinal.a = 1;
	return vTexture;
}
