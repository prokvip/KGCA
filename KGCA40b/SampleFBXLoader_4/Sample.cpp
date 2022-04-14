#include "Sample.h"
#include "TObjectMgr.h"
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

DWORD  Sample::LoadAllPath( const TCHAR* argv , std::vector<std::wstring>& list)
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
	}    while (FindNextFile(hFind, &ffd) != 0);

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
bool	Sample::Init()
{		
	std::vector<std::wstring> listname;
	// Greystone.fbx  LOD ¸Þ½¬ 5°³ 	
	listname.push_back(L"../../data/fbx/box1.fbx");
	listname.push_back(L"../../data/fbx/SM_Barrel.fbx");
	//listname.push_back(L"../../data/fbx/Greystone.fbx");
	//listname.push_back(L"../../data/fbx/idle.fbx");
	
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
	{
		TFbx* pFbx = &m_FbxObj[iObj];
		pFbx->Init();
		pFbx->m_pMainCamera = m_pMainCamera;
		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();
		pFbx->m_pMeshImp = I_ObjectMgr.Load(listname[iObj]);
		pFbx->m_DrawList.resize(pFbx->m_pMeshImp->m_DrawList.size());
		pFbx->SetPosition(T::TVector3(iObj * 100.0f, 0, 0));
		for (int iDraw = 0; iDraw < pFbx->m_pMeshImp->m_DrawList.size(); iDraw++)
		{
			pFbx->m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext.Get();
		}
	}

	//m_FbxObj[0].m_pAnimImporter = m_FbxObj[1].m_pMeshImp;
	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 25.0f, -50.0f),T::TVector3(0, 0.0f, 0));
	m_pMainCamera->CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 1000.0f);
	m_pLightTex = I_Texture.Load(L"../../data/pung00.dds");

	m_pNormalMap = I_Texture.Load(L"../../data/NormalMap/tileADOT3.jpg");

	return true;
}
bool	Sample::Frame()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Frame();
	}
	return true;
}
bool	Sample::Render()
{		
	m_pImmediateContext->PSSetShaderResources(
		1, 1, m_pLightTex->m_pSRV.GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(
		4, 1, m_pNormalMap->m_pSRV.GetAddressOf());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr,&m_pMainCamera->m_matView,&m_pMainCamera->m_matProj);
		m_FbxObj[iObj].Render();
	}
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();