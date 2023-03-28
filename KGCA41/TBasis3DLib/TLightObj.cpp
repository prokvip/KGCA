#include "TLightObj.h"
void TLightObj::SetPath(TCHAR* pPath)
{
	m_szPath = pPath;
}
HRESULT		TLightObj::Load(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;

	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), strFilePath);

	m_szName = strFilePath;
	return hr;
}
void TLightObj::SetValue(TVector3* pvPos,
	TVector3* pvDir,
	float* pfRange,
	TVector4* pvColor,
	TVector3* pvScale,
	TVector3* pvRotation,
	float fTheta,
	float fPhi)
{
	if (pvPos != NULL)	m_vInitPosition = *pvPos;
	if (pvDir != NULL)	m_vDirection = *pvDir;
	if (pfRange != NULL) m_fRange = *pfRange;
	if (pvColor != NULL) m_DiffuseColor = *pvColor;
	// 외부
	m_fTheta = cosf(XMConvertToRadians(fTheta / 2));
	// 내부 
	m_fPhi = cosf(XMConvertToRadians(fPhi / 2));

	if (pvScale != NULL) m_vScale = *pvScale;
	if (pvRotation != NULL) m_vRotation = *pvRotation;

	TMatrix matScale, matRotation;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&matRotation, m_vRotation.x, m_vRotation.y, m_vRotation.z);
	D3DXMatrixMultiply(&m_matInitWorld, &matScale, &matRotation);

	m_matInitWorld._41 = m_vInitPosition.x;
	m_matInitWorld._42 = m_vInitPosition.y;
	m_matInitWorld._43 = m_vInitPosition.z;
}

bool TLightObj::Init()
{
	SetValue();
	return true;
}
bool TLightObj::Update(TMatrix* pmatLightWorld)
{
	m_vPosition.x = pmatLightWorld->_41;
	m_vPosition.y = pmatLightWorld->_42;
	m_vPosition.z = pmatLightWorld->_43;

	D3DXVec3Normalize(&m_vDirection, &m_vPosition);
	m_vDirection *= -1.0f;

	return true;
}
bool TLightObj::Frame(float fElapaseTime, float fSecondPerFrame)
{
	// 2초당 1회전( 1 초 * XM_PI = 3.14 )
	float t = (float)fElapaseTime * XM_PI;

	TMatrix mRotation;
	D3DXMatrixRotationY(&mRotation, t*0.1f);
	D3DXMatrixMultiply(&m_matWorld, &m_matInitWorld, &mRotation);

	m_vPosition.x = m_matWorld._41;
	m_vPosition.y = m_matWorld._42;
	m_vPosition.z = m_matWorld._43;

	D3DXVec3Normalize(&m_vDirection, &m_vPosition);
	m_vDirection *= -1.0f;
	return true;
}

bool TLightObj::Render()
{
	return true;
}
bool TLightObj::Release()
{
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TLightObj::CreateResource()
{
	HRESULT hr = S_OK;
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT TLightObj::DeleteResource()
{
	HRESULT hr = S_OK;
	return hr;
}
TLightObj::TLightObj(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matInitWorld);

	m_vScale = TVector3(1.0f, 1.0f, 1.0f);
	m_vRotation = TVector3(0.0f, 0.0f, 0.0f);

	m_vInitPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_vPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_vDirection = TVector3(0.0f, -1.0f, 0.0f);

	m_fRange = 20.0f;
	m_fFalloff = 4.0f;

	m_DiffuseColor = TVector4(1.0f, 0.5f, 0.5f, 1.0f);

	m_fTheta = (float)(XM_PI / 4.0f);
	m_fPhi = m_fTheta + (float)(XMConvertToRadians(5));

	m_vOffset = TVector3(0.0f, 5.0f, 0.0f);

	m_szPath = _T("");
}

TLightObj::~TLightObj(void)
{
}

INT TLightMgr::Add(TVector3 vPosition, TVector3 vDir, const TCHAR *pFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TLightObj *pPoint = (TLightObj *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	TLightObj *pPoint = NULL;
	SAFE_NEW(pPoint, TLightObj);

	TCHAR szPath[MAX_PATH] = { 0, };
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);
	pPoint->SetPath(szPath);

	if (FAILED(pPoint->Load(m_pd3dDevice, szFileName)))
	{
		return 0;
	}
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	return m_iCurIndex;
}

bool TLightMgr::Init()
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Init();
	}
	return true;
}
bool TLightMgr::Frame()
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Frame();
	}
	return true;
}
bool TLightMgr::Render()
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Render();
	}
	return true;
}
bool TLightMgr::Update(TMatrix* matLightWorld)
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Update(matLightWorld);
	}
	return true;
}
bool TLightMgr::Release()
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Release();
	}
	TMap.clear();
	return true;
}
TLightObj* const TLightMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	TLightObj *pPoint = (*itor).second;
	return pPoint;
}
TLightObj* const  TLightMgr::GetPtr(T_STR strFindName)
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		TLightObj *pPoint = (TLightObj *)(*itor).second;
		if (pPoint->m_szName == strFindName)
		{
			return (*itor).second;
		}
	}
	return NULL;
}
void TLightMgr::SetDevice(ID3D11Device*	 pDevice)
{
	m_pd3dDevice = pDevice;
}
TLightMgr::TLightMgr(void)
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
}

TLightMgr::~TLightMgr(void)
{
	TLightObj *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TLightObj *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
}