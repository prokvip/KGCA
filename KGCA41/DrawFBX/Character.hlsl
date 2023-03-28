//#define HALF_VECTOR
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15

	float4 i : INDEX;
	float4 w : WEIGHT;
	float3 tan : TANGENT;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 3 -> 4
	float3 n : NORMAL;
	float4 c : COLOR0;    // COLOR0 ~ COLOR1
	float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15
	float3 r  : TEXCOORD1;
	float3 l  : TEXCOORD2;
	float3 e  : TEXCOORD3;
	float4 shadow  : TEXCOORD4;
};

// �������(����:�������� ����(float4)�� �Ҵ�Ǿ�� �Ѵ�.)
cbuffer cb0 : register(b0)
{
	// 1���� ��������(x,y,z,w)
	matrix   g_matWorld : packoffset(c0);
	matrix   g_matView : packoffset(c4);
	matrix   g_matProj : packoffset(c8);
	matrix	 g_matNormal : packoffset(c12);
	float4   Color0 : packoffset(c16);
	float    TimerX : packoffset(c17.x); // Timer.x, Timer.y, Timer.z, Timer.w	
};
cbuffer cb1 : register(b1)
{
	float4   vLightDir : packoffset(c0);
	float4   vLightPos : packoffset(c1);
	float4   vEyeDir   : packoffset(c2);
	float4   vEyePos   : packoffset(c3);
	matrix   g_matLight: packoffset(c4);
};
cbuffer cb2 : register(b2)
{
	float4x4 g_matBoneWorld[255]; //65535 / 4
}
VS_OUTPUT VS(VS_INPUT v)
{
	VS_OUTPUT pOut = (VS_OUTPUT)0;
	uint  iIndex = v.c.w;
	float4 vLocal = float4(v.p.xyz, 1.0f);// float4(v.p.x, v.p.y, v.p.z, 1.0f);
	//float4 vWorld = mul(vLocal, g_matBoneWorld[iIndex]);
	float4 vWorld = 0;
	float3 vNormal = 0;
	for (int iBone = 0; iBone < 4; iBone++)
	{
		uint iBoneIndex = v.i[iBone];
		vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * v.w[iBone];
		vNormal += mul(v.n, (float3x3)g_matBoneWorld[iBoneIndex]) * v.w[iBone];
	}

	vWorld = mul(vWorld, g_matWorld);
	vNormal = mul(vNormal, (float3x3)g_matNormal);
	pOut.shadow = mul(vWorld, g_matLight);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	pOut.p = vProj;
	pOut.n = normalize(vNormal);
	pOut.t = v.t;
	pOut.c = v.c;

	float3 T = normalize(mul(v.tan, (float3x3)g_matNormal));
	float3 B = normalize(cross(vNormal, T));

	float3x3 tanMat = { T.x, B.x, vNormal.x,
						T.y, B.y, vNormal.y,
						T.z, B.z, vNormal.z };
	pOut.l = normalize(mul(-vLightDir.xyz, tanMat));
	pOut.e = normalize(mul(vEyeDir.xyz, tanMat));
	pOut.r = reflect(vEyeDir.xyz, pOut.n);
	return pOut;
}
VS_OUTPUT VSColor(VS_INPUT v)
{
	VS_OUTPUT pOut = (VS_OUTPUT)0;
	float4 vLocal = float4(v.p.xyz, 1.0f);// float4(v.p.x, v.p.y, v.p.z, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	pOut.p = vProj;
	pOut.c = v.c;
	pOut.r = normalize(vLocal.xyz);
	return pOut;
}
Texture2D		g_txColor : register(t0);
Texture2D		g_txMask : register(t1);
TextureCube	    g_txCubeMap : register(t3);
Texture2D	    g_txNormalMap : register(t4);
Texture2D	    g_txShadowMap : register(t5);

SamplerState	g_Sample : register(s0);
SamplerState	g_SampleClamp : register(s1);

//DownColor * (1-up) + UpColor = DownColor + up * (UpColor - DownColor)
float3 CalcAmbient(float3 normal, float3 color) 
{
	const float3 AmbientUp = float3(1.0f, 1.0f, 1.0f);
	const float3 AmbientDown = float3(0.3f, 0.3f, 0.3f);
	const float3 AmbientRange = AmbientUp - AmbientDown;
	float up = normal.y * 0.5 + 0.5;
	float3 ambient = AmbientDown + up * AmbientRange;
	return ambient *color;
}

//float4 AmbientLightPS(VS_OUTPUT input) : SV_TARGET0
float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	float3 diffuse = g_txColor.Sample(g_SampleClamp, input.t).rgb;
	diffuse *= diffuse;
	float3 ambient = CalcAmbient(input.n, diffuse);
	return float4(ambient, 1.0);
}

float Specular(float3 vNormal)
{
	// Specular Lighting
	float  fPower = 0.0f;
#ifndef HALF_VECTOR
	float3 R = reflect(vLightDir.xyz, vNormal);
	fPower = pow(saturate(dot(R, -vEyeDir.xyz)), 5.0f);
#else
	float3 vHalf = normalize(-vLightDir.xyz + -vEyeDir.xyz);
	fPower = pow(saturate(dot(vNormal, vHalf)), 5.0f);
#endif	
	float4 specular = float4(fPower, fPower, fPower, 1.0f);
	return fPower;
}

float Diffuse(float3 vNormal)
{
	float fIntensity = max(0,
		dot(vNormal, normalize(-vLightDir.xyz)));
	//float4 diffuse = fIntensity;
	return fIntensity;
}
struct PBUFFER_OUTPUT
{
	float4 color0 : SV_TARGET0;
	float4 color1 : SV_TARGET1;
};

PBUFFER_OUTPUT PSMRT(VS_OUTPUT input) : SV_TARGET
{
	PBUFFER_OUTPUT output;
	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
	float4 color = g_txColor.Sample(g_Sample, input.t);
	float4 shadow = g_txShadowMap.Sample(g_SampleClamp, input.shadow.xy / input.shadow.w);
	uint id = shadow * 255.0f;
	if (id != (uint)TimerX && shadow.r > 0.0f)
	{
		color = color * float4(0.5f, 0.5f, 0.5f, 1);
	}
	else
	{
		color = color;
	}
	float4 normal = g_txNormalMap.Sample(g_Sample, input.t);
	normal = normalize((normal - 0.5f) * 2.0f);
	float  fDot = saturate(dot(normal.xyz, input.l));
	float4 final = color * float4(fDot, fDot, fDot, 1);
	// 0 ~ 1 -> 0.0f ~ 0.5f
	float2 uv = float2(fDot, 0.5f);
	//float4 mask = g_txMask.Sample(g_Sample, uv);
	// �ҽ�����(1) = ����ũ�̹����� �������κ��� ������ȴ�.
	// �ҽ�����(0) = ����ũ�̹����� ����κ���   ����ȴ�.
	//final = color*mask;// *mask;// *Color0;	
	//final = final * Specular(normal);
	//final.a = 1.0f;
	/* �����׽��� (���� ����� ���� ������ �� �� ���)
	 ���� : ������ �����ϱ� ����� ������Ʈ ��������
			���ĵ� ���¿� �����ϰ�  �������ȴ�.*/
			//if (final.a < 0.5f)
			//{
			//	discard;
			//}
	float4 vCube = g_txCubeMap.Sample(g_Sample, input.r);
	final = lerp(final, vCube, 0.5f);
	final.a = 1.0f;

	output.color0 = final;
	float3 vNormal = input.n * 0.5f + 0.5f;
	// �ʼ�->���ĺ��� =OFF;
	output.color1 = float4(vNormal, input.c.a);
	return output;
}
//float4 PS(VS_OUTPUT input) : SV_TARGET
//{	
//	//�ؽ�ó���� t��ǥ�� �ش��ϴ� �÷���(�ȼ�) ��ȯ
//	float4 color = g_txColor.Sample(g_Sample, input.t);
//	float4 normal = g_txNormalMap.Sample(g_Sample, input.t);
//	normal = normalize((normal - 0.5f) * 2.0f);
//	float  fDot = saturate(dot(normal.xyz, input.l));
//	float4 final = color * float4(fDot,fDot, fDot, 1);
//	// 0 ~ 1 -> 0.0f ~ 0.5f
//	float2 uv = float2(fDot, 0.5f);
//	//float4 mask = g_txMask.Sample(g_Sample, uv);
//	// �ҽ�����(1) = ����ũ�̹����� �������κ��� ������ȴ�.
//	// �ҽ�����(0) = ����ũ�̹����� ����κ���   ����ȴ�.
//	//final = color*mask;// *mask;// *Color0;	
//	final = final * Specular(normal);
//	final.a = 1.0f;
//	/* �����׽��� (���� ����� ���� ������ �� �� ���)
//	 ���� : ������ �����ϱ� ����� ������Ʈ ��������
//			���ĵ� ���¿� �����ϰ�  �������ȴ�.*/
//			//if (final.a < 0.5f)
//			//{
//			//	discard;
//			//}
//	float4 vCube = g_txCubeMap.Sample(g_Sample, input.r);
//	final = lerp(final, vCube, 0.5f);
//	final.a = 1.0f;
//	return color;
//}

float4 PSAlphaBlend(VS_OUTPUT input) : SV_TARGET
{
	float4 color = g_txColor.Sample(g_Sample, input.t);
	float4 final = color * input.c;
	final.a = color.a;
	return final;
}

float4 PSColor(VS_OUTPUT input) : SV_TARGET
{
	float4 vColor = input.c;
	vColor.a = 0.5f * input.c.a;
	return vColor;
}