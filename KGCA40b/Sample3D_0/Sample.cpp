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
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/HEIGHT_MOUNTAIN.bmp");// heightMap513.bmp");
	TTexture* pTexMap = I_Texture.Load(L"../../data/map/020.bmp");
	m_MapObj.m_pColorTex = pTexMap;	
	m_MapObj.m_pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"map.hlsl", "VS");;
	m_MapObj.m_pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"map.hlsl", "PS");;
	// Á¤Á¡°³¼ö ( 2n½Â+1)
	m_MapObj.CreateMap(m_MapObj.m_iNumCols, m_MapObj.m_iNumRows, 20.0f);
	if (!m_MapObj.Create(m_pd3dDevice.Get(),m_pImmediateContext.Get()))
	{
		return false;
	}

	m_PlayerObj.Init();
	m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;
	m_PlayerObj.SetPosition(TVector3(0.0f, 1.0f, 0.0f));
	if (!m_PlayerObj.Create(m_pd3dDevice.Get(),	m_pImmediateContext.Get()))
	{
		return false;
	}	

	m_SkyObj.Init();	
	m_SkyObj.SetPosition(TVector3(0.0f, 0.0f, 0.0f));	
	if (!m_SkyObj.Create(	m_pd3dDevice.Get(), 
							m_pImmediateContext.Get(),
							L"sky.hlsl",
							L"../../data/sky/xxx.bmp"))
	{
		return false;
	}
	
	//m_ObjB.Init();
	//m_ObjB.m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	//m_ObjB.m_pVShader = pVShader;
	//m_ObjB.m_pPShader = pPShader;
	//if (!m_ObjB.Create(m_pd3dDevice.Get(),
	//	m_pImmediateContext.Get()))
	//{
	//	return false;
	//}
	//m_ObjB.m_matWorld.Translation(1.0f, 0.0f, 0.0f);

	return true;
}
bool	Sample::Frame()
{	
	if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vLight *  g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vLight * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('W') || TInput::Get().GetKey(VK_UP))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}
	TMatrix matRotate;
	TMatrix matScale;
	static float fRadian = 0.0f;
	fRadian += (TInput::Get().m_ptDeltaMouse.x / (float)g_rtClient.right)* TBASIS_PI;
	matRotate.YRotate(fRadian);
	matScale.Scale(50, 50, 50);
	m_PlayerObj.m_matWorld = matScale*matRotate;
	m_PlayerObj.m_vPos.y= m_MapObj.GetHeight(m_PlayerObj.m_vPos.x, m_PlayerObj.m_vPos.z)+50;
	m_PlayerObj.SetPosition(m_PlayerObj.m_vPos);

	m_Camera.m_vTarget = m_PlayerObj.m_vPos;

	float y = m_MapObj.GetHeight(m_Camera.m_vCamera.x, m_Camera.m_vCamera.z);
	m_Camera.m_vCamera = m_PlayerObj.m_vPos +
						m_PlayerObj.m_vLook * -1.0f *5.0f +
						m_PlayerObj.m_vUp * 5.0f;

	m_Camera.Frame();
	m_MapObj.Frame();
	m_PlayerObj.Frame();
	return true;
}
bool	Sample::Render()
{	
	m_SkyObj.m_matViewSky = m_Camera.m_matView;
	m_SkyObj.m_matViewSky._41 = 0;
	m_SkyObj.m_matViewSky._42 = 0;
	m_SkyObj.m_matViewSky._43 = 0;
	TMatrix matRotation, matScale;
	matScale.Scale(3000.0f, 3000.0f, 3000.0f);
	matRotation.YRotate(g_fGameTimer*0.00f);
	m_SkyObj.m_matWorld = matScale * matRotation;	
	m_SkyObj.SetMatrix(NULL, &m_SkyObj.m_matViewSky, &m_Camera.m_matProj);
	m_pImmediateContext->RSSetState(TDxState::g_pRSNoneCullSolid);
	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::m_pSSPoint);
	m_SkyObj.Render();

	//m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);
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
	m_SkyObj.Release();
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