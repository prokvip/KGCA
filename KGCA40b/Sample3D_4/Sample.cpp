#include "Sample.h"
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void    Sample::CreateMapObject()
{
	srand(time(NULL));
	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"Box.hlsl", "PS");

	m_pBoxObj = new TBoxObj;
	m_pBoxObj->m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	m_pBoxObj->m_pVShader = pVShader;
	m_pBoxObj->m_pPShader = pPShader;
	if (!m_pBoxObj->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return ;
	}

	T::TMatrix matScale;
	T::TMatrix matRotateObj;
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		TMapObject* pObj = new TMapObject;
		for (int iv = 0; iv < 8; iv++)
		{
			pObj->box.vList[iv] = m_pBoxObj->m_BoxCollision.vList[iv];
		}
		pObj->vPos = T::TVector3(
			randstep(m_MapObj.m_BoxCollision.vMin.x, m_MapObj.m_BoxCollision.vMax.x),
			0.0f,
			randstep(m_MapObj.m_BoxCollision.vMin.z, m_MapObj.m_BoxCollision.vMax.z));
		
		T::D3DXMatrixScaling(&matScale, randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f));
		T::D3DXMatrixRotationYawPitchRoll(&matRotateObj,
			cosf(randstep(0.0f, 360.0f)) * XM_PI,
			sinf(randstep(0.0f, 360.0f)) * XM_PI,
			1.0f);
		pObj->matWorld = matScale * matRotateObj;
		pObj->vPos.y = m_MapObj.GetHeight(pObj->vPos.x, pObj->vPos.z);
		pObj->matWorld._41 = pObj->vPos.x;
		pObj->matWorld._42 = pObj->vPos.y;
		pObj->matWorld._43 = pObj->vPos.z;
		pObj->UpdateData();
		pObj->UpdateCollision();
		pObj->pObject = m_pBoxObj;
		m_pObjList.push_back(pObj);
	}
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
	m_Camera.CreateViewMatrix(  T::TVector3(0, 500.0f, -100.0f),
								T::TVector3(0, 0.0f, 0));
	m_Camera.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 2000.0f);
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
	m_PlayerObj.Init();
	m_PlayerObj.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;
	m_PlayerObj.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_PlayerObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}

	m_SkyObj.Init();
	m_SkyObj.SetPosition(T::TVector3(0.0f, 0.0f, 0.0f));
	if (!m_SkyObj.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get(),
		L"sky.hlsl",
		L"../../data/sky/LobbyCube.dds"))
	{
		return false;
	}


	m_MapObj.Init();
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/129.jpg");
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
	m_Quadtree.m_pCamera = &m_Camera;
	m_Quadtree.Build(&m_MapObj, 5);
	CreateMapObject();
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		m_Quadtree.AddObject(m_pObjList[iObj]);
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
	m_Quadtree.Update(&m_Camera);
	m_PlayerObj.Frame();
	return true;
}
bool	Sample::Render()
{		
	m_SkyObj.SetMatrix(NULL, &m_Camera.m_matView, &m_Camera.m_matProj);	
	m_SkyObj.Render();

	if (m_bWireFrame)
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullWireFrame);
	else
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	m_MapObj.SetMatrix(nullptr, &m_Camera.m_matView,&m_Camera.m_matProj);
	//m_MapObj.Render();	
	m_Quadtree.Render();	

	m_PlayerObj.SetMatrix(nullptr, &m_Camera.m_matView,	&m_Camera.m_matProj);
	m_PlayerObj.Render();		
	
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));


	//MiniMapRender();
	return true;
}

bool    Sample::MiniMapRender()
{
	/*for (int iNode = 0; iNode < m_Quadtree.g_pDrawLeafNodes.size(); iNode++)
	{
			m_MapObj.SetMatrix(nullptr, &m_CameraTopView.m_matView,
										&m_CameraTopView.m_matProj);
			m_MapObj.m_ConstantList.Color = T::TVector4(0, 1, 0, 1);
			m_MapObj.m_pContext->UpdateSubresource(
				m_MapObj.m_pConstantBuffer, 0, NULL, &m_MapObj.m_ConstantList, 0, 0);

			m_MapObj.m_pContext->IASetIndexBuffer(
				m_Quadtree.g_pDrawLeafNodes[iNode]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			m_MapObj.m_pContext->DrawIndexed(m_Quadtree.g_pDrawLeafNodes[iNode]->m_IndexList.size(), 0, 0);
			
	}*/
	for (auto obj : m_pObjList)
	{
		obj->pObject->SetMatrix(&obj->matWorld, &m_CameraTopView.m_matView,
				&m_CameraTopView.m_matProj);
		obj->pObject->m_ConstantList.Color = T::TVector4(0, 0, 0, 1);
		obj->pObject->Render();
	}
	/*for (auto pObj : m_pObjList)
	{
		pObj->pObject->SetMatrix(&pObj->matWorld,  &m_Camera.m_matView,
								&m_Camera.m_matProj);
		if (m_Camera.ClassifyOBB(&pObj->box) == TRUE)
		{
			pObj->pObject->Render();
		}
	}*/
	m_Camera.SetMatrix(nullptr, &m_CameraTopView.m_matView,	&m_CameraTopView.m_matProj);
	m_Camera.Render();
	return true;
}
bool	Sample::Release()
{
	m_SkyObj.Release();
	m_MapObj.Release();
	m_PlayerObj.Release();
	m_Camera.Release();

	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		if (m_pObjList[iObj] != nullptr)
		{
			delete m_pObjList[iObj];
			m_pObjList[iObj] = nullptr;
		}
	}
	if (m_pBoxObj != nullptr)
	{
		m_pBoxObj->Release();
		delete m_pBoxObj;
		m_pBoxObj = nullptr;
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();