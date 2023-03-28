#include "TDxShader.h"
static const unsigned int MAX_INCLUDES = 9;
struct sInclude
{
	HANDLE         hFile;
	HANDLE         hFileMap;
	LARGE_INTEGER  FileSize;
	void           *pMapData;
};

class CIncludeHandler : public ID3DInclude
{
private:
	struct sInclude   m_includeFiles[MAX_INCLUDES];
	unsigned int      m_nIncludes;

public:
	CIncludeHandler()
	{
		// array initialization
		for (unsigned int i = 0; i<MAX_INCLUDES; i++)
		{
			m_includeFiles[i].hFile = INVALID_HANDLE_VALUE;
			m_includeFiles[i].hFileMap = INVALID_HANDLE_VALUE;
			m_includeFiles[i].pMapData = NULL;
		}
		m_nIncludes = 0;
	}
	~CIncludeHandler()
	{
		for (unsigned int i = 0; i<m_nIncludes; i++)
		{
			UnmapViewOfFile(m_includeFiles[i].pMapData);

			if (m_includeFiles[i].hFileMap != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[i].hFileMap);

			if (m_includeFiles[i].hFile != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[i].hFile);
		}

		m_nIncludes = 0;
	}

	STDMETHOD(Open(
		D3D_INCLUDE_TYPE IncludeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID *ppData,
		UINT *pBytes
		))
	{
		unsigned int   incIndex = m_nIncludes + 1;

		// Make sure we have enough room for this include file
		if (incIndex >= MAX_INCLUDES)
			return E_FAIL;

		// try to open the file
		m_includeFiles[incIndex].hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (INVALID_HANDLE_VALUE == m_includeFiles[incIndex].hFile)
		{
			return E_FAIL;
		}

		// Get the file size
		GetFileSizeEx(m_includeFiles[incIndex].hFile, &m_includeFiles[incIndex].FileSize);

		// Use Memory Mapped File I/O for the header data
		m_includeFiles[incIndex].hFileMap = CreateFileMappingA(m_includeFiles[incIndex].hFile, NULL, PAGE_READONLY, m_includeFiles[incIndex].FileSize.HighPart, m_includeFiles[incIndex].FileSize.LowPart, pFileName);
		if (m_includeFiles[incIndex].hFileMap == NULL)
		{
			if (m_includeFiles[incIndex].hFile != INVALID_HANDLE_VALUE)
				CloseHandle(m_includeFiles[incIndex].hFile);
			return E_FAIL;
		}

		// Create Map view
		*ppData = MapViewOfFile(m_includeFiles[incIndex].hFileMap, FILE_MAP_READ, 0, 0, 0);
		*pBytes = m_includeFiles[incIndex].FileSize.LowPart;

		// Success - Increment the include file count
		m_nIncludes = incIndex;

		return S_OK;
	}

	STDMETHOD(Close(LPCVOID pData))
	{
		// Defer Closure until the container destructor 
		return S_OK;
	}
};

void TDxShader::Apply(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->HSSetShader(m_pHullShader, NULL, 0);
	pd3dContext->DSSetShader(m_pDomainShader, NULL, 0);
	pd3dContext->GSSetShader(m_pGeometryShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	pd3dContext->CSSetShader(m_pComputeShader, NULL, 0);
}

HRESULT		TDxShader::Load(ID3D11Device* pd3dDevice,
	TCHAR* pLoadShaderFile,
	CHAR* pEntryPoint,
	CHAR* pCompiler,
	STAGE_TYPE type,
	ID3DBlob* pLoadBlob)
{
	HRESULT hr = S_OK;
	if (pLoadBlob == NULL)
	{
		const D3D_SHADER_MACRO defines[] =
		{
			"EXAMPLE_DEFINE", "1",
			NULL, NULL
		};

		// 스크립트 로드해서 컴파일
		DWORD dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;

		if (FAILED(hr = D3DCompileFromFile(
			pLoadShaderFile,//L"VertexShader.vsh",
			defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			pEntryPoint,//"VS",
			pCompiler, //"vs_5_0",
			dwShaderFlags,
			NULL, 
			&m_pBuffer, &pErrorBlob)))
		{
			//DXTRACE_ERR_MSGBOX( DXGetErrorDescription(hr),hr);
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			SAFE_RELEASE(pErrorBlob);
			return hr;
		}
	}
	switch (type)
	{
	case t_VS:
	{
		V_RETURN(pd3dDevice->CreateVertexShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pVertexShader));

		D3DGetInputSignatureBlob(m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			&m_pBlobVS);
	}break;
	case t_HS:
	{
		V_RETURN(pd3dDevice->CreateHullShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pHullShader));

	}break;
	case t_DS:
	{
		V_RETURN(pd3dDevice->CreateDomainShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pDomainShader));

	}break;

	case t_GS:
	{
		V_RETURN(pd3dDevice->CreateGeometryShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pGeometryShader));

	}break;
	case t_PS:
	{
		V_RETURN(pd3dDevice->CreatePixelShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pPixelShader));

	}break;
	case t_CS:
	{
		V_RETURN(pd3dDevice->CreateComputeShader(
			m_pBuffer->GetBufferPointer(),
			m_pBuffer->GetBufferSize(),
			NULL,
			&m_pComputeShader));

	}break;
	}

	return hr;
};
void TDxShader::ApplyVS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	else
		pd3dContext->VSSetShader(NULL, NULL, 0);
}
void TDxShader::ApplyHS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->HSSetShader(m_pHullShader, NULL, 0);
	else
		pd3dContext->HSSetShader(NULL, NULL, 0);
}
void TDxShader::ApplyDS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->DSSetShader(m_pDomainShader, NULL, 0);
	else
		pd3dContext->DSSetShader(NULL, NULL, 0);
}
void TDxShader::ApplyGS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->GSSetShader(m_pGeometryShader, NULL, 0);
	else
		pd3dContext->GSSetShader(NULL, NULL, 0);
}
void TDxShader::ApplyPS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	else
		pd3dContext->PSSetShader(NULL, NULL, 0);
}
void TDxShader::ApplyCS(ID3D11DeviceContext*	pd3dContext, bool bUse)
{
	if (bUse)
		pd3dContext->CSSetShader(m_pComputeShader, NULL, 0);
	else
		pd3dContext->CSSetShader(NULL, NULL, 0);
}
HRESULT TDxShader::CompileShaderFromFile(const WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
	WCHAR *strLastSlash = NULL;
	bool  resetCurrentDir = false;

	// Get the current working directory so we can restore it later
	UINT nBytes = GetCurrentDirectory(MAX_PATH, workingPath);
	if (nBytes == MAX_PATH)
	{
		return E_FAIL;
	}

	wcscpy_s(str, MAX_PATH, szFileName);
	// Check we can find the file first
	// V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, szFileName ) );

	// Check if the file is in the current working directory
	wcscpy_s(filePath, MAX_PATH, szFileName);

	strLastSlash = wcsrchr(filePath, TEXT('\\'));
	if (strLastSlash)
	{
		// Chop the exe name from the exe path
		*strLastSlash = 0;

		SetCurrentDirectory(filePath);
		resetCurrentDir = true;
	}

	// open the file
	HANDLE hFile = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	// Get the file size
	LARGE_INTEGER FileSize;
	GetFileSizeEx(hFile, &FileSize);

	// create enough space for the file data
	BYTE* pFileData = new BYTE[FileSize.LowPart];
	if (!pFileData)
		return E_OUTOFMEMORY;

	// read the data in
	DWORD BytesRead;
	if (!ReadFile(hFile, pFileData, FileSize.LowPart, &BytesRead, NULL))
		return E_FAIL;

	CloseHandle(hFile);

	// Create an Include handler instance
	CIncludeHandler* pIncludeHandler = new CIncludeHandler;

	// Compile the shader using optional defines and an include handler for header processing
	ID3DBlob* pErrorBlob;
	hr = D3DCompile(pFileData, FileSize.LowPart, "none", NULL, static_cast< ID3DInclude* > (pIncludeHandler),
		szEntryPoint, szShaderModel, flags, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, ppBlobOut, &pErrorBlob);

	delete pIncludeHandler;
	delete[]pFileData;

	// Restore the current working directory if we need to 
	if (resetCurrentDir)
	{
		SetCurrentDirectory(workingPath);
	}


	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return hr;
}
HRESULT TDxShader::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}
HRESULT TDxShader::Load(ID3D11Device* pDevice, const TCHAR* strShaderPath,
	LPCSTR strVSEntryPoint,
	LPCSTR strPSEntryPoint,
	LPCSTR strShaderModel,
	UINT Flags)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif  	
	ID3DBlob* pPSBuffer = NULL;
	V_RETURN(CompileShaderFromFile(strShaderPath, strVSEntryPoint, "vs_5_0", &m_pBlobVS));
	V_RETURN(CompileShaderFromFile(strShaderPath, strPSEntryPoint, "ps_5_0", &pPSBuffer));
	V_RETURN(pDevice->CreateVertexShader(m_pBlobVS->GetBufferPointer(),
		m_pBlobVS->GetBufferSize(), NULL, &m_pVertexShader));
	V_RETURN(pDevice->CreatePixelShader(pPSBuffer->GetBufferPointer(),
		pPSBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	pPSBuffer->Release();
	//SetInputLayout( pDevice, m_pBlobVS );
	m_szName = strShaderPath;
	return S_OK;
}

bool TDxShader::Release()
{
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pComputeShader);
	SAFE_RELEASE(m_pBuffer);
	return true;
}

void TDxShader::ClearShaderResources(ID3D11DeviceContext*  pImmediateContext)
{
	// unbind resources
	ID3D11ShaderResourceView* pNULLViews[8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	pImmediateContext->VSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->GSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(1, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(2, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(3, 8, pNULLViews);
}
TDxShader::TDxShader(void)
{
	m_pBuffer = NULL;
	m_szPath = Res(L"shader");
	m_pVertexShader = NULL;//정점쉐이더	
	m_pHullShader = NULL;//헐 쉐이더
	m_pDomainShader = NULL;//도메인쉐이더
	m_pGeometryShader = NULL;//기하쉐이더
	m_pPixelShader = NULL;//픽쉘쉐이더
	m_pComputeShader = NULL;//컴퓨터쉐이더
	m_pBlobVS = 0;
}

TDxShader::~TDxShader(void)
{
	Release();
}
INT	TShaderMgr::Add(TCHAR* pLoadShaderFile,
	CHAR* pEntryPoint,
	CHAR* pCompiler,
	STAGE_TYPE type)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pLoadShaderFile == NULL || pEntryPoint == NULL) return -1;
	ID3DBlob* pLoadBlob = NULL;

	// 중복 방지 
	if (pLoadShaderFile)
	{
		_tsplitpath_s(pLoadShaderFile, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TDxShader *pPoint = (TDxShader *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				pLoadBlob = pPoint->m_pBuffer;
				break;
			}
		}
	}

	TDxShader *pPoint = NULL;
	SAFE_NEW(pPoint, TDxShader);
	pPoint->Load(m_pd3dDevice, pLoadShaderFile, pEntryPoint, pCompiler, type, pLoadBlob);
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	pPoint->m_szName = szFileName;
	return m_iCurIndex;
};
INT TShaderMgr::Add(const TCHAR *pLoadShaderFile, LPCSTR strVSEntryPoint,
	LPCSTR strPSEntryPoint,
	LPCSTR strShaderModel)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pLoadShaderFile == NULL) return -1;
	ID3DBlob* pLoadBlob = NULL;

	if (pLoadShaderFile)
	{
		_tsplitpath_s(pLoadShaderFile, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TDxShader *pPoint = (TDxShader *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				pLoadBlob = pPoint->m_pBuffer;
				break;// return (*itor).first;
			}
		}
	}


	TDxShader *pPoint = NULL;
	SAFE_NEW(pPoint, TDxShader);
	pPoint->Load(m_pd3dDevice, pLoadShaderFile, strVSEntryPoint, strPSEntryPoint, strShaderModel);

	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	pPoint->m_szName = szFileName;
	return m_iCurIndex;
}
bool TShaderMgr::Release()
{
	TDxShader *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TDxShader *)(*itor).second;
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
TDxShader* TShaderMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	TDxShader *pPoint = (*itor).second;
	return pPoint;
}
void TShaderMgr::SetDevice(ID3D11Device*	 pDevice)
{
	m_pd3dDevice = pDevice;
}
bool TShaderMgr::Delete(INT iDelete)
{
	TemplateMapItor itor = TMap.find(iDelete);
	if (itor == TMap.end()) return true;
	TDxShader *pPoint = (*itor).second;
	if (pPoint)
	{
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.erase(itor);
	return true;
}
TShaderMgr::TShaderMgr(void)
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
}

TShaderMgr::~TShaderMgr(void)
{
	TDxShader *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TDxShader *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();

	m_iCurIndex = 0;
}
