#include "Sample.h"
bool Sample::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, (unsigned int)_countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, (unsigned int)_countof(pTemp), &iNumFrame);
		//m_rtSpriteList[iCnt].resize(iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}
bool Sample::Init()
{
	GameDataLoad(L"SpriteInfo.txt");
	for (int isp = 0; isp < m_rtSpriteList.size(); isp++)
	{
		TSprite* pEffect = new TSprite;
		m_pSpriteList.push_back(pEffect);
	}
	std::wstring shaderfilename = L"../../data/shader/DefaultShapeMask.txt";
	std::wstring mapshader = L"../../data/shader/DefaultShape.txt";
	TTexture* pMaskTex = I_Tex.Load(L"../../data/bitmap2.bmp");

	m_pMap = new TMapObject;
	m_pMap->Create(m_pd3dDevice,
		m_pImmediateContext,
		mapshader,
		L"../../data/kgcabk.bmp");//"L"../../data/gameHeight.png");
	m_pMap->SetRect({ 0, 0,	2000.0f,2000.0f });
	m_pMap->SetPosition({ 0.0f, 0.0f });

	m_pObject = new TSprite;
	m_pObject->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/bitmap1.bmp");
	m_pObject->SetMask(pMaskTex);
	m_pObject->m_fSpeed = 300.0f;
	RECT rt = m_rtSpriteList[0][0];
	TRect tRt;
	tRt.x1 = rt.left;
	tRt.y1 = rt.top;
	tRt.w = rt.right;
	tRt.h = rt.bottom;
	m_pObject->SetRect(tRt);
	m_pObject->SetPosition({ 400,300 });

	

	m_pUser = new TUser2D;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext,
		L"../../data/shader/DefaultShapeMask.txt",
		L"../../data/bitmap1.bmp");
	m_pUser->SetMask(pMaskTex);
	m_pUser->m_fSpeed = 300.0f;
	m_pUser->SetRect({ 91, 2, 39, 59 });
	m_pUser->SetPosition({ 400,300 });

	m_vCamera = m_pUser->m_vPos;
	m_vCamera.y -= 200.0f;
	return true;
}
bool Sample::Frame()
{	
	m_pUser->SetCameraSize({ 800, 800 });
	m_pUser->SetCameraPos(m_vCamera);
	m_pUser->Frame();
	m_vCamera = m_pUser->m_vPos;
	m_vCamera.y -= 200.0f;

	m_pMap->SetCameraSize({ 800, 800 });
	m_pMap->SetCameraPos(m_vCamera);
	m_pMap->Frame();
	static int iSpriteIndex = 1;
	if (I_Input.GetKey('1') == KEY_PUSH)
	{
		iSpriteIndex++;
		if (iSpriteIndex > 2) iSpriteIndex = 0;
	}
	static float eFffectRunning = 1.33f;
	static UINT index = 0;
	static float fEffectTimer = 0.0f;
	fEffectTimer += g_fSecondPerFrame;
	float fStep = eFffectRunning / (float)m_rtSpriteList[iSpriteIndex].size();
	if(fEffectTimer >= fStep)
	{
		fEffectTimer -= fStep;
		index++;	
	}

	if (index >= m_rtSpriteList[iSpriteIndex].size())
	{
		index = 0;
	}
	RECT rt = m_rtSpriteList[iSpriteIndex][index];
	TRect tRt;
	tRt.x1 = rt.left;
	tRt.y1 = rt.top;
	tRt.w = rt.right;
	tRt.h = rt.bottom;
	m_pObject->SetRect(tRt);
	m_pObject->SetPosition({ (float)I_Input.m_ptPos.x,(float)I_Input.m_ptPos.y });
	return true;
}
bool Sample::Render()
{	
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	m_pMap->Render();

	m_pUser->PreRender();
	m_pImmediateContext->PSSetShaderResources(1, 1,
		&m_pUser->m_pMaskTex->m_pTextureSRV);
	m_pUser->PostRender();

	m_pObject->PreRender();
		m_pImmediateContext->PSSetShaderResources(1, 1,
			&m_pObject->m_pMaskTex->m_pTextureSRV);
	m_pObject->PostRender();
	return true;
}
bool Sample::Release()
{	
	m_pMap->Release();
	delete m_pMap;
	m_pMap = nullptr;

	m_pUser->Release();
	delete m_pUser;
	m_pUser = nullptr;

	m_pObject->Release();
	delete m_pObject;
	m_pObject = nullptr;
	return true;
}

GAME_RUN(TestWriter, 800,600)