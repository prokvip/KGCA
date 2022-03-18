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
	

	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");
	
	m_ObjA.Init();
	m_ObjA.m_pColorTex = pTex;
	m_ObjA.m_pVShader = pVShader;
	m_ObjA.m_pPShader = pPShader;
	if (!m_ObjA.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get()))
	{
		return false;
	}
	// world
	//TMatrix matRotate, matScale, matTrans;
	//matRotate.ZRotate(g_fGameTimer);
	//matScale.Scale(cos(g_fGameTimer) * 0.5f + 0.5f, 1.0f, 1.0f);
	//matTrans.Translation(0,	cos(g_fGameTimer) * 0.5f + 0.5f, 0);
	m_ObjA.m_matWorld.Translation(0.0f, 0.0f, 3.0f);

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
	return true;
}
bool	Sample::Frame()
{	
	m_Camera.Frame();

	m_ObjA.SetMatrix(nullptr, &m_Camera.m_matView,&m_Camera.m_matProj);
	m_ObjB.SetMatrix(nullptr, &m_Camera.m_matView,&m_Camera.m_matProj);
	m_ObjA.Frame();
	m_ObjB.Frame();
	return true;
}
bool	Sample::Render()
{	
	m_ObjB.Render();
	m_ObjA.Render();
	
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	m_ObjA.Release();
	m_ObjB.Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();