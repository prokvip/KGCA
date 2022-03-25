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

	m_CameraTopView.CreateViewMatrix(T::TVector3(0, 3000.0f, -1),
									 T::TVector3(0, 0.0f, 0));
	m_CameraTopView.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 30000.0f);

	m_Camera.Init();
	m_Camera.CreateViewMatrix(  T::TVector3(0, 300.0f, -100.0f),
								T::TVector3(0, 0.0f, 0));
	m_Camera.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 1000.0f);
	m_Camera.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_Camera.m_pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VSColor");;
	m_Camera.m_pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PSColor");;
	m_Camera.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_Camera.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}
	//

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
	m_PlayerObj.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_PlayerObj.Create(m_pd3dDevice.Get(),	m_pImmediateContext.Get()))
	{
		return false;
	}	

	m_SkyObj.Init();	
	m_SkyObj.SetPosition(T::TVector3(0.0f, 0.0f, 0.0f));	
	if (!m_SkyObj.Create(	m_pd3dDevice.Get(), 
							m_pImmediateContext.Get(),
							L"sky.hlsl",
							L"../../data/sky/LobbyCube.dds"))
	{
		return false;
	}
	
	m_ObjList.resize(100);
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		m_ObjList[iObj].Init();
		m_ObjList[iObj].m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
		m_ObjList[iObj].m_pVShader = pVShader;
		m_ObjList[iObj].m_pPShader = pPShader;
		m_ObjList[iObj].SetPosition(
			T::TVector3(-300.0f + rand()%600,
						100.0f, 
						-300.0f + rand() % 600));
		if (!m_ObjList[iObj].Create(m_pd3dDevice.Get(),
			m_pImmediateContext.Get()))
		{
			return false;
		}		
	}
	return true;
}
bool	Sample::Frame()
{	
	T::TVector2 dir = TInput::Get().GetDelta();
	/*if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vRight *  g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vRight * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('W') || TInput::Get().GetKey(VK_UP))
	{
		m_PlayerObj.m_vPos += m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_PlayerObj.m_vPos -= m_PlayerObj.m_vLook * g_fSecPerFrame * 10.0f;
	}*/
	T::TMatrix matRotate;
	T::TMatrix matScale;

	T::D3DXMatrixRotationY(&matRotate, -dir.y);
	T::D3DXMatrixScaling(&matScale, 50, 50, 50);
	m_PlayerObj.m_matWorld = matScale*matRotate;
	m_PlayerObj.m_vPos.y= m_MapObj.GetHeight(m_PlayerObj.m_vPos.x, m_PlayerObj.m_vPos.z)+50;
	m_PlayerObj.SetPosition(m_PlayerObj.m_vPos);
	m_Camera.m_vTarget = m_PlayerObj.m_vPos;
	float y = m_MapObj.GetHeight(m_Camera.m_vCamera.x, m_Camera.m_vCamera.z);
	/*m_Camera.m_vCamera = m_PlayerObj.m_vPos +
						m_PlayerObj.m_vLook * -1.0f *5.0f +
						m_PlayerObj.m_vUp * 5.0f;*/

	if (TInput::Get().GetKey('A') || TInput::Get().GetKey(VK_LEFT))
	{
		m_Camera.MoveSide(-g_fSecPerFrame * 100.0f);
	}
	if (TInput::Get().GetKey('D') || TInput::Get().GetKey(VK_RIGHT))
	{
		m_Camera.MoveSide(g_fSecPerFrame * 100.0f);
	}
	//m_Camera.MoveLook(10.0f);
	if (TInput::Get().GetKey('W') )
	{
		m_Camera.MoveLook(g_fSecPerFrame * 100.0f);
	}
	if (TInput::Get().GetKey('S') || TInput::Get().GetKey(VK_DOWN))
	{
		m_Camera.MoveLook(-g_fSecPerFrame * 100.0f);
	}
	
	m_Camera.Update(T::TVector4(-dir.x, -dir.y,0,0));
	m_MapObj.Frame();
	m_PlayerObj.Frame();

	T::TMatrix matRotateObj;
	for (auto& obj : m_ObjList)
	{
		T::D3DXMatrixScaling(&matScale, 
			10 * cosf(g_fGameTimer),
			10 * cosf(g_fGameTimer), 
			10 * cosf(g_fGameTimer));
		T::D3DXMatrixRotationYawPitchRoll(&matRotateObj, 
			    cosf(g_fGameTimer*obj.m_vPos.x*0.001f) * XM_PI,
				sinf(g_fGameTimer*obj.m_vPos.y * 0.001f) * XM_PI,
			    1.0f);
		obj.m_matWorld = matScale * matRotateObj;
		obj.m_vPos.y = m_MapObj.GetHeight(obj.m_vPos.x, obj.m_vPos.z) + 50;
		obj.SetPosition(obj.m_vPos);
		obj.Frame();
	}
	return true;
}
bool	Sample::Render()
{	
	m_SkyObj.m_matViewSky = m_Camera.m_matView;
	m_SkyObj.m_matViewSky._41 = 0;
	m_SkyObj.m_matViewSky._42 = 0;
	m_SkyObj.m_matViewSky._43 = 0;
	T::TMatrix matRotation, matScale;
	T::D3DXMatrixScaling(&matScale, 10.0f, 10.0f, 10.0f);
	T::D3DXMatrixRotationY(&matRotation, g_fGameTimer * 0.00f);
	m_SkyObj.m_matWorld = matScale * matRotation;	
	m_SkyObj.SetMatrix(NULL, &m_SkyObj.m_matViewSky, &m_Camera.m_matProj);
	m_pImmediateContext->RSSetState(TDxState::g_pRSNoneCullSolid);
	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::m_pSSPoint);
	m_SkyObj.Render();

	//m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);
	m_MapObj.SetMatrix(nullptr, &m_Camera.m_matView,&m_Camera.m_matProj);
	m_MapObj.Render();
	
	m_PlayerObj.SetMatrix(nullptr, &m_Camera.m_matView,	&m_Camera.m_matProj);
	m_PlayerObj.Render();	

	for (auto& obj : m_ObjList)
	{
		obj.SetMatrix(nullptr,  &m_Camera.m_matView,
								&m_Camera.m_matProj);
		if (m_Camera.ClassifyOBB(&obj.m_BoxCollision) == TRUE)
		{
			obj.Render();
		}
		else
		{
			obj.SetMatrix(nullptr, &m_CameraTopView.m_matView,
									&m_CameraTopView.m_matProj);
			obj.m_ConstantList.Color = T::TVector4(0, 0, 0, 1);
			obj.Render();
		}
	}

	m_Camera.SetMatrix(nullptr, &m_CameraTopView.m_matView, 
								&m_CameraTopView.m_matProj);
	m_Camera.Render();	
	
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
	m_Camera.Release();
	for (auto& obj : m_ObjList)
	{
		obj.Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();