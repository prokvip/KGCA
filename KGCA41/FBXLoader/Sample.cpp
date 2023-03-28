#pragma once
#include "Sample.h"
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

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
bool Sample::LoadBindPoseObject()
{
	// skeleton & animatin
	m_BindPoseObj1 = std::make_shared<TFbxObj>();
	m_BindPoseObjIdle1 = std::make_shared<TFbxObj>();
	//Greystone.fbx  LOD 메쉬 5개
	if (!m_BindPoseObj1->Load(GetDevice(), "../../data/fbx/Greystone.fbx", L"Shader.fx"))
	{
		return false;
	}
	if (!m_BindPoseObjIdle1->Load(GetDevice(), "../../data/fbx/Idle.fbx", L"Shader.fx"))
	{
		return false;
	}
	m_BindPoseObj1->m_matControlWorld.Translation(TVector3(-100, 0, 0));

	m_BindPoseObj1->SetSkeletonMesh(m_BindPoseObjIdle1.get());
	m_ObjList.push_back(std::move(m_BindPoseObj1));
	//m_ObjList.push_back(std::addressof(m_BindPoseObj));
	
	return true;
}
bool Sample::LoadUE4PoseObject()
{
	// skeleton & animatin
	m_BindPoseObj2 = std::make_shared<TFbxObj>();
	m_BindPoseObjIdle2 = std::make_shared<TFbxObj>();
	m_BindPoseObjWalk2 = std::make_shared<TFbxObj>();
	m_BindPoseObjRun2 = std::make_shared<TFbxObj>();
	if (!m_BindPoseObj2->Load(GetDevice(), "../../data/fbx/SK_Mannequin.fbx", L"Shader.fx"))
	{
		return false;
	}
	if (!m_BindPoseObjIdle2->Load(GetDevice(), "../../data/fbx/ThirdPersonIdle.fbx", L"Shader.fx"))
	{
		return false;
	}
	if (!m_BindPoseObjWalk2->Load(GetDevice(), "../../data/fbx/ThirdPersonWalk.fbx", L"Shader.fx"))
	{
		return false;
	}
	if (!m_BindPoseObjRun2->Load(GetDevice(), "../../data/fbx/ThirdPersonRun.fbx", L"Shader.fx"))
	{
		return false;
	}
	m_BindPoseObj2->SetSkeletonMesh(m_BindPoseObjWalk2.get());
	//m_ObjList.push_back(std::move(m_BindPoseObj));
	//m_ObjList.push_back(std::addressof(m_BindPoseObj));
	return true;
}
bool Sample::Init()
{
	std::vector<std::wstring> listname;
	LoadAllPath(L"../../data/fbx/AdvancedVillagePack/Meshes", listname);

	const char* fbxobject[] =
	{	
		"../../data/fbx/man.fbx",
		//"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx",
		//"../../data/fbx/SM_Tree_Var01.fbx",
		//"../../data/fbx/Scifi_Model_L2_all_in_one.fbx",
		/*"../../data/fbx/st00.fbx",		
		"../../data/fbx/st00.fbx",				
		"../../data/fbx/Scifi_Model_L2_all_in_one.fbx",
		"../../data/fbx/SphereBoxZ.fbx",		
		"../../data/fbx/charMob.fbx",
		"../../data/fbx/swatrifle_0.fbx",
		"../../data/fbx/man.fbx",*/
		//"../../data/fbx/sphere.fbx",
		//"../../data/fbx/SK_Mannequin.fbx",
		//"../../data/fbx/Greystone.fbx",
	};
	for (int iObj = 0; iObj < ARRAYSIZE(fbxobject); iObj++)
	{
		std::shared_ptr<TFbxObj>	fbxobj = make_shared<TFbxObj>();
		if (fbxobj->Load(GetDevice(), fbxobject[iObj], L"Shader.fx"))
		{
			fbxobj->m_matControlWorld.Translation(TVector3(iObj*30.0f, 0, 0));
			m_ObjList.push_back(std::move(fbxobj));
		}
	}

	// bind pos character
	LoadBindPoseObject();  // or
	LoadUE4PoseObject();	 // or
	//--------------------------------------------------------------------------------------
	// 카메라  행렬 
	//--------------------------------------------------------------------------------------
	SAFE_NEW(m_pMainCamera, TCamera);
	m_pMainCamera->SetViewMatrix(TVector3(0, 100.0f, -100.0f), TVector3(m_ObjList.size()*30.0f,0.0f, 100));
	float fAspect = m_SwapChainDesc.BufferDesc.Width / (float)(m_SwapChainDesc.BufferDesc.Height);
	m_pMainCamera->SetProjMatrix(XM_PI * 0.25f, fAspect, 1.0f, 30000.0f);
	m_pGameCamera = m_pMainCamera;
	return true;
}
bool Sample::Render()
{
	DX::ApplyBS(m_pImmediateContext, DX::TDxState::g_pNoAlphaBlend);
	for (auto obj : m_ObjList)
	{
		obj->m_vLight = m_pMainCamera->m_vLookVector;
		obj->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		obj->Render(m_pImmediateContext);
	}
	if (m_BindPoseObj2)
	{
		m_BindPoseObj2->m_vLight = m_pMainCamera->m_vLookVector;
		m_BindPoseObj2->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_BindPoseObj2->Render(m_pImmediateContext);
	}
	return true;
}
bool Sample::Release()
{
	for (auto obj : m_ObjList)
	{
		obj->Release();
	}
	if (m_BindPoseObj1)
	{
		if (m_BindPoseObj1 != nullptr)
		{
			m_BindPoseObj1->Release();
		}
		m_BindPoseObj1->Release();
	}
	if (m_BindPoseObj2)
	{
		if (m_BindPoseObjIdle2 != nullptr)
		{
			m_BindPoseObjIdle2->Release();
		}
		if (m_BindPoseObjWalk2 != nullptr)
		{
			m_BindPoseObjWalk2->Release();
		}
		if (m_BindPoseObjRun2 != nullptr)
		{
			m_BindPoseObjRun2->Release();
		}
		if (m_BindPoseObj2 != nullptr)
		{
			m_BindPoseObj2->Release();
		}
	}
	for (auto obj = m_ObjList.begin(); obj != m_ObjList.end();)
	{
		obj = m_ObjList.erase(obj);
	}
	m_ObjList.clear();
	return true;
}
bool Sample::Frame()
{
	m_pMainCamera->Frame();
	for (auto obj : m_ObjList)
	{
		obj->Frame();
	}
	if (m_BindPoseObj2)
	{
		if (g_InputData.bSpace)
		{
			m_BindPoseObj2->SetSkeletonMesh(m_BindPoseObjRun2.get());
		}
		else if (g_InputData.bWKey || g_InputData.bSKey || g_InputData.bAKey || g_InputData.bDKey)
		{
			m_BindPoseObj2->SetSkeletonMesh(m_BindPoseObjWalk2.get());
		}
		else
		{
			m_BindPoseObj2->SetSkeletonMesh(m_BindPoseObjIdle2.get());
		}
		m_BindPoseObj2->Frame();
	}
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
{
	HRESULT hr = S_OK;
	if (m_pMainCamera)
	{
		float fAspectRatio = m_SwapChainDesc.BufferDesc.Width / (float)m_SwapChainDesc.BufferDesc.Height;
		m_pMainCamera->SetProjMatrix(XM_PI / 4, fAspectRatio, 0.1f, 500.0f);
	}
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteResource()
{
	HRESULT hr = S_OK;
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}

Sample::Sample(void)
{
	m_pMainCamera = nullptr;
}

Sample::~Sample(void)
{
}
TBASIS_RUN(L"TBasisSample FBXLoader");

