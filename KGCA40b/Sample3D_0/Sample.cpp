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
	
	m_Obj.Init();
	m_Obj.m_pColorTex = pTex;
	m_Obj.m_pVShader = pVShader;
	m_Obj.m_pPShader = pPShader;
	if (!m_Obj.Create(m_pd3dDevice.Get(), 
		m_pImmediateContext.Get()))
	{
		return false;
	}
	return true;
}
bool	Sample::Frame()
{	
	m_Obj.Frame();
	return true;
}
bool	Sample::Render()
{
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDSSDepthEnable, 0x00);
	m_Obj.Render();
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	m_Obj.Release();
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();