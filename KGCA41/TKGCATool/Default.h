#define g_iNumLight 1
Texture2D    g_txTex			: register(t0);
Texture2D    g_txTex1 : register(t1);
Texture2D    g_txTex2 : register(t2);
Texture2D    g_txTex3 : register(t3);
Texture2D    g_txTex4 : register(t4);
Texture2D    g_txTex5 : register(t5);
//Texture2D    g_txTexArray[16] : register(t6);

SamplerState g_SampleWrap		: register(s0);

cbuffer cb_data : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);	
	matrix g_matWorldInverse : packoffset(c12);

	float3  g_vLightDir : packoffset(c16);
	float  g_fRadius1 : packoffset(c16.w);

	float3  g_vLightPos : packoffset(c17);
	float  g_fRadius2 : packoffset(c17.w);

	float3  g_vEyeDir : packoffset(c18);
	float  g_fRadius3 : packoffset(c18.w);

	float3  g_vEyePos : packoffset(c19);
	float  g_fRadius4 : packoffset(c19.w);

	float  g_fValue1 : packoffset(c20.x);
	float  g_fValue2 : packoffset(c20.y);
	float  g_fValue3 : packoffset(c20.z);
	float  g_fTimer : packoffset(c20.w);
};

//cbuffer cb_dataLight : register(b1)
//{
//	float4  g_vLightDir[g_iNumLight];
//	float4  g_vLightPos[g_iNumLight];
//	float4  g_vLightColor[g_iNumLight];
//};

float4 ComputePointDiffuseLight(float3 vVertexPos, 
						 float3 vVertexNormal, 
						 int iNumLight)
{
	float4 vAmbintColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 vPointLightColor = float4(0, 0, 0, 1);
	float4 vLightColor = float4(1, 1, 1, 1);
	////for (int iLight = 0; iLight < iNumLight; iLight++)
	//{
		// diffuse
		float4 vLight;
		vLight.xyz = normalize(vVertexPos - g_vLightPos.xyz);
		vLight.w = distance(vVertexPos,g_vLightPos.xyz);
		float fLuminace = smoothstep(vLight.w-5.0f, vLight.w, g_fRadius1);
		float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));
		vPointLightColor += float4(vLightColor.rgb * fLuminace* fIntensity, 1.0f);
			
		//specular
		
	//}
	return vPointLightColor +vAmbintColor;
}

float4 ComputeSpotDiffuseLight(float3 vVertexPos,
	float3 vVertexNormal,
	int iNumLight)
{
	float4 vAmbintColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 vPointLightColor = float4(0, 0, 0, 1);
	float4 vLightInColor = float4(1, 0, 0, 1);
	float4 vLightOutColor = float4(1, 0, 0, 1);
	////for (int iLight = 0; iLight < iNumLight; iLight++)
	//{
		float4 vLight;
		vLight.xyz = normalize(vVertexPos - g_vLightPos.xyz);
		vLight.w = distance(vVertexPos, g_vLightPos.xyz);
		float fDot = dot(g_vLightDir.xyz, vLight.xyz);
		float fLuminace1 = smoothstep(vLight.w - 5, vLight.w, g_fRadius2);
		float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));
		if (fDot >= g_fRadius4)
		{
			float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));
			vPointLightColor += float4(vLightInColor.rgb * min(fIntensity,fLuminace1), 1.0f);
		}
		else
		{
			if (fDot >= g_fRadius3)
			{
				float fLuminace2 = smoothstep(g_fRadius3, g_fRadius4, fDot);
				vPointLightColor += float4(vLightOutColor.rgb * min(min(fLuminace2,fLuminace1), fIntensity), 1.0f);
			}
		}
	//}
	return vPointLightColor +vAmbintColor;
}

float4 ComputePointSpecularLight(float3 vVertexPos,
	float3 vVertexNormal,
	int iNumLight)
{
	float4 vAmbintColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 vPointLightColor = float4(0, 0, 0, 1);
	float4 vLightColor = float4(1, 1, 1, 1);
	////for (int iLight = 0; iLight < iNumLight; iLight++)
	//{
	float4 vLight;
	vLight.xyz = reflect(g_vLightDir.xyz, vVertexNormal);
	vLight.w = distance(vVertexPos, g_vLightPos.xyz);
	float fLuminace = smoothstep(vLight.w - 5.0f, vLight.w, g_fRadius1);
	float fIntensity = pow(saturate(dot(-g_vEyeDir.xyz, vLight.xyz)), 30.0f);
	vPointLightColor += float4(vLightColor.rgb * fLuminace * fIntensity, 1.0f);

//}
	return vPointLightColor + vAmbintColor;
}

float4 ComputeSpotSpecularLight(float3 vVertexPos,
	float3 vVertexNormal,
	int iNumLight)
{
	float4 vAmbintColor = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 vPointLightColor = float4(0, 0, 0, 1);
	float4 vLightInColor = float4(1, 0, 0, 1);
	float4 vLightOutColor = float4(1, 0, 0, 1);
	////for (int iLight = 0; iLight < iNumLight; iLight++)
	//{
	float4 vLight;
	vLight.xyz = reflect(g_vLightDir.xyz, vVertexNormal);
	vLight.w = distance(vVertexPos, g_vLightPos.xyz);
	float fDot = dot(g_vLightDir.xyz, vLight.xyz);
	float fLuminace1 = smoothstep(vLight.w - 5, vLight.w, g_fRadius2);
	float fIntensity = pow(saturate(dot(-g_vEyeDir.xyz, vLight.xyz)), 30.0f);
	if (fDot >= g_fRadius4)
	{
		float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));
		vPointLightColor += float4(vLightInColor.rgb * min(fIntensity, fLuminace1), 1.0f);
	}
	else
	{
		if (fDot >= g_fRadius3)
		{
			float fLuminace2 = smoothstep(g_fRadius3, g_fRadius4, fDot);
			vPointLightColor += float4(vLightOutColor.rgb * min(min(fLuminace2, fLuminace1), fIntensity), 1.0f);
		}
	}
	//}
	return vPointLightColor + vAmbintColor;
}