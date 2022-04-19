#include "Sample.h"
#include "TObjectMgr.h"
#include <strsafe.h>
#include "TBoxObj.h"

DWORD  Sample::LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	StringCchLength(argv, MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 3))
	{
		//_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	StringCchCopy(szDir, MAX_PATH, argv);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
		return dwError;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			std::wstring path = argv;
			path += L"/";
			path += ffd.cFileName;
			list.push_back(path);
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	return dwError;
}


void Sample::DisplayErrorBox(const WCHAR* lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool	Sample::LoadMap()
{
	m_MapObj.Init();
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/heightMap513.bmp");
	m_MapObj.CreateMap(m_MapObj.m_iNumCols, m_MapObj.m_iNumRows, 100.0f);
	if (!m_MapObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
		L"MapRT.hlsl",
		L"../../data/map/002.jpg"))
	{
		return false;
	}
	m_Quadtree.m_pCamera = m_pMainCamera;
	m_Quadtree.Build(&m_MapObj, 5);
	/*CreateMapObject();
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		m_Quadtree.AddObject(m_pObjList[iObj]);
	}*/
	return true;
}
bool    Sample::LoadFbx()
{
	std::vector<std::wstring> listname;
	// Greystone.fbx  LOD 메쉬 5개 
	listname.push_back(L"../../data/fbx/Greystone.fbx");
	listname.push_back(L"../../data/fbx/idle.fbx");
	//listname.push_back(L"../../data/fbx/Man.fbx");
	//LoadAllPath(L"../../data/fbx/AdvancedVillagePack/Meshes", listname);

	// 0 ~ 60  idel
	// 61 ~91  walk;
	// 92 ~ 116	  run
	// 120 ~205 jump
	// 	205 ~289  attack
	//listname.push_back(L"../../data/fbx/ManSkin.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelIdel.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelWalk.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/SM_Barrel.fbx");	
	//listname.push_back(L"../../data/fbx/st00sc00.fbx");
	//listname.push_back(L"../../data/fbx/SM_Tree_Var01.fbx");
	//listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");
	//listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");

	I_ObjectMgr.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	//for (int iObj = 0; iObj < 0; iObj++)
	{
		TFbx* pFbx = &m_FbxObj[iObj];
		pFbx->Init();
		pFbx->m_pMainCamera = m_pMainCamera;
		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();
		pFbx->m_pMeshImp = I_ObjectMgr.Load(listname[iObj]);
		pFbx->m_DrawList.resize(pFbx->m_pMeshImp->m_DrawList.size());
		int iRow = iObj / 10;
		int iCol = iObj / 10;
		int iOffRow = iObj % 10;
		int iOffCol = iObj % 10;
		float fHeight = m_MapObj.TMap::GetHeight(m_FbxObj[iObj].m_vPos.x, m_FbxObj[iObj].m_vPos.z);
		pFbx->SetPosition(T::TVector3(iOffCol * 300.0f, fHeight, iRow * 300.0f));		
		for (int iDraw = 0; iDraw < pFbx->m_pMeshImp->m_DrawList.size(); iDraw++)
		{
			pFbx->m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext.Get();
		}
	}

	m_FbxObj[0].m_pAnimImporter = m_FbxObj[1].m_pMeshImp;
	return true;
}
bool	Sample::Init()
{
	HRESULT hr;
	if (FAILED(m_QuadObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"Quad.hlsl")))
	{
		MessageBox(0, _T("m_QuadObj 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pShadowPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), 
		L"Character.hlsl", "PSColor");

	LoadMap();
	LoadFbx();


	m_QuadObj.CreateTextures(m_pd3dDevice.Get(), 
			m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
	m_QuadObj.SetBuffer(m_pd3dDevice.Get());
	m_QuadObj.ComputeKernel(9);

	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 500.0f, -500.0f), 
									m_FbxObj[0].m_vPos);
	m_pMainCamera->CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);
	
	m_pLightTex = I_Texture.Load(L"../../data/pung00.dds");
	m_pNormalMap = I_Texture.Load(L"../../data/NormalMap/tileADOT3.jpg");

	m_vLightPos = TVector3(0, 1000, 10);
	T::D3DXVec3Normalize(&m_vLightDir, &m_vLightDir);
	m_dxRT.Create(m_pd3dDevice.Get(), 2048, 2048);
	m_pProjShadowVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(),
		L"ProjShadow.hlsl", "VS");
	m_pProjShadowPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(),
		L"ProjShadow.hlsl", "PS");


	m_matTex = TMatrix(   0.5f, 0.0f, 0.0f, 0.0f
							, 0.0f, -0.5f, 0.0f, 0.0f
							, 0.0f, 0.0f, 1.0f, 0.0f
							, 0.5f, 0.5f, 0.0f, 1.0f);
	return true;
}
bool	Sample::Frame()
{
	TMatrix matRotation;
	TVector3 vLight = m_vLightPos;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&m_vLightDir, &m_vLightDir);

	m_QuadObj.Frame();
	m_MapObj.Frame();
	m_Quadtree.Update(m_pMainCamera);
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Frame();
	}

	TVector4 vClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (m_dxRT.Begin(m_pImmediateContext.Get(), vClearColor))
	{
		//-----------------------------------------------------
		// 1패스:그림자맵 생성
		//-----------------------------------------------------		
		TVector3 vEye = vLight;
		TVector3 vLookat = TVector3(0, 0, 0);
		TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&m_matViewLight, &vEye, &vLookat, &vUp);
		D3DXMatrixPerspectiveFovLH(&m_matProjLight, XM_PI / 2, 1, 0.1f, 5000.0f);
		RenderShadow(&m_matViewLight, &m_matProjLight);		
		m_dxRT.End(m_pImmediateContext.Get());
	}

	if (TInput::Get().GetKey('8')==KEY_PUSH)
	{
		TTextureMgr::SaveFile(m_pImmediateContext.Get(), m_dxRT.m_pTexture.Get(), L"proj.bmp");
	}
	return true;
}
void Sample::RenderShadow(TMatrix* matView, TMatrix* matProj)
{
	ApplyDSS(m_pImmediateContext.Get(), TDxState::g_pDSSDepthEnable);
	ApplyRS(m_pImmediateContext.Get(), TDxState::g_pRSBackCullSolid);
	
	//m_MapObj.m_bAlphaBlend = false;
	//m_MapObj.SetMatrix(nullptr, matProj, matProj);
	//m_Quadtree.PreRender();
	//m_pImmediateContext.Get()->VSSetShader(m_pProjShadowVShader->m_pVertexShader, NULL, 0);
	//m_pImmediateContext.Get()->PSSetShader(m_pProjShadowPShader->m_pPixelShader, NULL, 0);
	//m_Quadtree.PostRender();

	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr, matView, matProj);
		m_FbxObj[iObj].RenderShadow(m_pProjShadowPShader);
	}	
}
bool	Sample::Render()
{
	RenderIntoBuffer(m_pImmediateContext.Get());
	
	ApplySS(m_pImmediateContext.Get(), TDxState::m_pSSLinear);
	ApplySS(m_pImmediateContext.Get(), TDxState::g_pSSClampLinear);
	m_QuadObj.SetMatrix(nullptr, nullptr, nullptr);
	m_QuadObj.Render();


//#ifdef _DEBUG
//	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
//	{
//		for (int iDraw = 0; iDraw < m_FbxObj[iObj].m_DrawList.size(); iDraw++)
//		{
//			g_pBoxDebug->SetMatrix(&m_FbxObj[iObj].m_pMeshImp->m_DrawList[iDraw]->m_matWorld,
//				&m_pMainCamera->m_matView,
//				&m_pMainCamera->m_matProj);
//			g_pBoxDebug->DrawDebugRender(&m_FbxObj[iObj].m_pMeshImp->m_DrawList[iDraw]->m_BoxCollision);
//		}
//	}
//#endif
//	std::wstring msg = L"FPS:";
//	msg += std::to_wstring(m_GameTimer.m_iFPS);
//	msg += L"  GT:";
//	msg += std::to_wstring(m_GameTimer.m_fTimer);
//	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));

	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	return true;
}
bool	Sample::Release()
{
	m_dxRT.Release();
	m_QuadObj.Release();
	m_MapObj.Release();

	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
void Sample::RenderIntoBuffer(ID3D11DeviceContext* pContext)
{
	// get the old render targets
	ID3D11RenderTargetView* pOldRTV;
	ID3D11DepthStencilView* pOldDSV;
	pContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);

	// 이전 백버퍼 랜더링 결과를 사용(스카이박스)하기 위해서 알파사용.
	float color[4] ={	0, 0, 0, 0	};
	pContext->ClearRenderTargetView(m_QuadObj.m_pColorRTV.Get(), color);
	pContext->ClearRenderTargetView(m_QuadObj.m_pNormalDepthRTV.Get(), color);
	pContext->ClearDepthStencilView(m_QuadObj.m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	// Set the new render targets
	ID3D11RenderTargetView* pViews[2];
	pViews[0] = m_QuadObj.m_pColorRTV.Get();
	pViews[1] = m_QuadObj.m_pNormalDepthRTV.Get();
	pContext->OMSetRenderTargets(2, pViews, m_QuadObj.m_pDepthStencilView.Get());

	// Render the particles
	RenderMRT(pContext);

	// restore the original render targets
	pViews[0] = pOldRTV;
	pViews[1] = NULL;
	pContext->OMSetRenderTargets(2, pViews, pOldDSV);
	SAFE_RELEASE(pOldRTV);
	SAFE_RELEASE(pOldDSV);
}
void Sample::RenderMRT(ID3D11DeviceContext* pContext)
{
	pContext->OMSetDepthStencilState(TDxState::g_pDSSDepthEnable, 0x00);
	ApplyBS(pContext, TDxState::m_AlphaBlendDisable);
	
	m_MapObj.m_bAlphaBlend = false;
	m_MapObj.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_MapObj.m_LightConstantList.matLight = m_matViewLight * m_matProjLight * m_matTex;
	T::D3DXMatrixTranspose( &m_MapObj.m_LightConstantList.matLight,
							&m_MapObj.m_LightConstantList.matLight);
	m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::g_pSSClampLinear);
	m_Quadtree.PreRender();	
		pContext->PSSetShaderResources(1, 1,m_dxRT.m_pSRV.GetAddressOf());
	m_Quadtree.PostRender();


	if (m_pLightTex)
		m_pImmediateContext->PSSetShaderResources(1, 1, m_pLightTex->m_pSRV.GetAddressOf());
	if (m_pNormalMap)
		m_pImmediateContext->PSSetShaderResources(4, 1, m_pNormalMap->m_pSRV.GetAddressOf());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_FbxObj[iObj].Render();
		
		TVector4 pLight = TVector4(m_vLightDir.x, m_vLightDir.y, m_vLightDir.z, 1.0f);
		TPlane pPlane = TPlane(0, 1, 0, -(m_FbxObj[iObj].m_vPos.y+1.1f));
		TVector4 p(pPlane.x, pPlane.y, pPlane.z, pPlane.w);
		TMatrix matShadow;
		D3DXMatrixShadow(&matShadow, &pLight, &pPlane);

		TMatrix matSaveWorld = m_FbxObj[iObj].m_matWorld;
			matShadow = m_FbxObj[iObj].m_matWorld * matShadow;
			m_FbxObj[iObj].SetMatrix(&matShadow, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			m_FbxObj[iObj].RenderShadow(m_pShadowPShader);
		m_FbxObj[iObj].m_matWorld = matSaveWorld;
	}
	ClearD3D11DeviceContext(pContext);
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();