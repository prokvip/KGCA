#include "Sample.h"
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool	Sample::Init()
{	
	m_Camera.Init();

	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");
	
	m_MapObj.Init();
	m_MapObj.m_pColorTex = pTex;
	m_MapObj.m_pVShader = pVShader;
	m_MapObj.m_pPShader = pPShader;
	TMatrix matRotate, matScale;
	matRotate.XRotate(DegreeToRadian(90));
	matScale.Scale(100, 100.0f, 100.0f);
	m_MapObj.m_matWorld = matScale * matRotate;
	if (!m_MapObj.Create(m_pd3dDevice.Get(),m_pImmediateContext.Get()))
	{
		return false;
	}

	m_PlayerObj.Init();
	m_PlayerObj.m_pColorTex = pTex;
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;
	m_PlayerObj.SetPosition(TVector3(0.0f, 1.0f, 0.0f));
	if (!m_PlayerObj.Create(m_pd3dDevice.Get(),	m_pImmediateContext.Get()))
	{
		return false;
	}	
	

	m_ObjB.Init();
	m_ObjB.m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	m_ObjB.m_pVShader = pVShader;
	m_ObjB.m_pPShader = pPShader;
	if (!m_ObjB.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get()))
	{
		return false;
	}
	m_ObjB.m_matWorld.Translation(1.0f, 0.0f, 0.0f);

	// world
	//TMatrix matRotate, matScale, matTrans;
	//matRotate.ZRotate(g_fGameTimer);
	//matScale.Scale(cos(g_fGameTimer) * 0.5f + 0.5f, 1.0f, 1.0f);
	//matTrans.Translation(0,	cos(g_fGameTimer) * 0.5f + 0.5f, 0);
	return true;
}
bool	Sample::Frame()
{	
	if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_PlayerObj.m_vPos.x -= g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_PlayerObj.m_vPos.x += g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('W') || TInput::Get().GetKey(VK_UP))
	{
		m_PlayerObj.m_vPos.z += g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_PlayerObj.m_vPos.z -= g_fSecPerFrame * 10.0f;
	}
	m_PlayerObj.SetPosition(m_PlayerObj.m_vPos);

	m_Camera.m_vTarget = m_PlayerObj.m_vPos;
	m_Camera.m_vCamera = m_PlayerObj.m_vPos + TVector3(0,30.0f,-25.0f);

	m_Camera.Frame();
	m_MapObj.Frame();
	m_PlayerObj.Frame();
	return true;
}
bool	Sample::Render()
{	
	m_MapObj.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_MapObj.Render();
	m_PlayerObj.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_PlayerObj.Render();	
	/*m_ObjB.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_ObjB.Render();*/
	
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	m_MapObj.Release();
	m_PlayerObj.Release();
	m_ObjB.Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();