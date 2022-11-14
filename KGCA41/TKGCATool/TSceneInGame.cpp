#include "pch.h"
#include "TSceneInGame.h"
#include "TInput.h"
static TVector2 vSize = { 800, 800 };

void TSceneInGame::AddEffect()
{
	TEffect2D* pEffect = new TEffect2D;
	UINT iEffect = rand() % 3;
	if (iEffect == 0)
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtExplosion");
	else if (iEffect == 1)
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtBomb");
	else
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtClash");
	pEffect->m_fEffectTimer = 0.0f;
	RECT rt = pEffect->m_pSprite->m_uvArray[0];
	pEffect->m_tRect.x1 = rt.left;
	pEffect->m_tRect.y1 = rt.top;
	pEffect->m_tRect.w = rt.right;
	pEffect->m_tRect.h = rt.bottom;

	pEffect->m_iIndex = 0;
	pEffect->m_fLifeTime = 1.0f;
	pEffect->m_vPos = { (float)I_Input.m_ptPos.x,(float)I_Input.m_ptPos.y };
	pEffect->m_fStep = pEffect->m_fLifeTime /
		pEffect->m_pSprite->m_uvArray.size();
	pEffect->m_iMaxIndex = pEffect->m_pSprite->m_uvArray.size();

	pEffect->m_pSprite->SetRect(pEffect->m_tRect);
	pEffect->m_pSprite->SetPosition(pEffect->m_vPos);

	pEffect->m_rtCollision = pEffect->m_pSprite->m_rtCollision;
	pEffect->m_rtCollision.x1 = pEffect->m_vPos.x;
	pEffect->m_rtCollision.y1 = pEffect->m_vPos.y;
	m_pEffectList.push_back(pEffect);
}
void TSceneInGame::AddEffect(float x, float y)
{
	TEffect2D* pEffect = new TEffect2D;
	UINT iEffect = rand() % 3;
	if (iEffect == 0)
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtExplosion");
	else if (iEffect == 1)
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtBomb");
	else
		pEffect->m_pSprite = I_Sprite.GetPtr(L"rtClash");
	pEffect->m_fEffectTimer = 0.0f;
	RECT rt = pEffect->m_pSprite->m_uvArray[0];
	pEffect->m_tRect.x1 = rt.left;
	pEffect->m_tRect.y1 = rt.top;
	pEffect->m_tRect.w = rt.right;
	pEffect->m_tRect.h = rt.bottom;

	pEffect->m_iIndex = 0;
	pEffect->m_fLifeTime = 1.0f;
	pEffect->m_vPos = { x,y };
	pEffect->m_fStep = pEffect->m_fLifeTime /
		pEffect->m_pSprite->m_uvArray.size();
	pEffect->m_iMaxIndex = pEffect->m_pSprite->m_uvArray.size();

	pEffect->m_pSprite->SetRect(pEffect->m_tRect);
	pEffect->m_pSprite->SetPosition(pEffect->m_vPos);

	pEffect->m_rtCollision = pEffect->m_pSprite->m_rtCollision;
	pEffect->m_rtCollision.x1 = pEffect->m_vPos.x;
	pEffect->m_rtCollision.y1 = pEffect->m_vPos.y;
	m_pEffectList.push_back(pEffect);
}
void TSceneInGame::AddProjectile(TVector2 pos)
{
	TProjectile2D* pEffect = new TProjectile2D;
	pEffect->m_pSprite = I_Sprite.GetPtr(L"rtProjectile");
	pEffect->m_fEffectTimer = 0.0f;
	RECT rt = pEffect->m_pSprite->m_uvArray[0];
	pEffect->m_tRect.x1 = rt.left;
	pEffect->m_tRect.y1 = rt.top;
	pEffect->m_tRect.w = rt.right;
	pEffect->m_tRect.h = rt.bottom;

	pEffect->m_iIndex = 0;
	pEffect->m_fLifeTime = 10.0f;
	pEffect->m_vPos = pos;
	pEffect->m_fStep = pEffect->m_fLifeTime /
		pEffect->m_pSprite->m_uvArray.size();
	pEffect->m_iMaxIndex = pEffect->m_pSprite->m_uvArray.size();

	pEffect->m_pSprite->SetRect(pEffect->m_tRect);
	pEffect->m_pSprite->SetPosition(pEffect->m_vPos);

	pEffect->m_rtCollision = pEffect->m_pSprite->m_rtCollision;
	pEffect->m_rtCollision.x1 = pEffect->m_vPos.x;
	pEffect->m_rtCollision.y1 = pEffect->m_vPos.y;
	m_Projectile.push_back(pEffect);
}
bool TSceneInGame::Init()
{	
	I_Sprite.Load(L"ui.txt");

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

	m_pUser = new TUser2D;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext,
		L"../../data/shader/DefaultShapeMask.txt",
		L"../../data/bitmap1.bmp");
	m_pUser->SetMask(pMaskTex);
	m_pUser->m_fSpeed = 300.0f;
	m_pUser->SetRect({ 90, 2, 40, 60 });
	m_pUser->SetPosition({ 0.0f,0.0f });

	m_vCamera = m_pUser->m_vPos;
	//m_vCamera.y -= 200.0f;

	for (int iNpc = 0; iNpc < 100; iNpc++)
	{
		TNpc2D* npc = new TNpc2D;
		npc->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../data/shader/DefaultShapeMask.txt",
			L"../../data/bitmap1.bmp");
		if (iNpc % 2 == 0)
		{
			npc->SetRect({ 46, 62, 68, 79 });
		}
		else
		{
			npc->SetRect({ 115, 62, 37, 35 });
		}
		npc->SetDirection({ randstep(-1.0f, 1.0f),	randstep(-1.0f, 1.0f) });

		npc->SetCameraPos(m_vCamera);
		npc->SetPosition({ randstep(-900,+900), randstep(-900,+900) });
		npc->SetMask(pMaskTex);
		m_pNpcList.push_back(npc);
	}
	return true;
}
bool TSceneInGame::Frame()
{
	float fAspectRatio = vSize.y / vSize.x;
	if (I_Input.GetKey(VK_LEFT) == KEY_HOLD)
	{		
		vSize.x -= g_fSecondPerFrame * 100.0f;
		vSize.y -= g_fSecondPerFrame * 100.0f * fAspectRatio;
	}
	if (I_Input.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		vSize.x += g_fSecondPerFrame * 100.0f;
		vSize.y += g_fSecondPerFrame * 100.0f * fAspectRatio;
	}
	m_pUser->Frame();
	m_vCamera = m_pUser->m_vPos;
	//m_vCamera.y -= 200.0f;
	m_pUser->SetCameraSize(vSize);
	m_pUser->SetCameraPos(m_vCamera);
	m_pUser->SetPosition(m_pUser->m_vPos, m_vCamera);

	m_pMap->SetCameraSize(vSize);
	m_pMap->SetCameraPos(m_vCamera);
	m_pMap->Frame();

	for (auto iter = m_pNpcList.begin();
		iter != m_pNpcList.end(); iter++)
	{
		TNpc2D* npc = *iter;
		npc->SetCameraSize(vSize);
		npc->SetCameraPos(m_vCamera);
		npc->Frame();
	}

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		AddProjectile(m_pUser->m_vPos);
	}

	for (auto iter = m_Projectile.begin(); iter != m_Projectile.end(); )
	{
		TProjectile2D* pEffect = *iter;
		pEffect->m_vPos.x = m_pUser->m_vPos.x;
		if (pEffect->Update() == false)
		{
			delete pEffect;
			iter = m_Projectile.erase(iter);
			continue;
		}
		iter++;
	}
	for (auto iter = m_Projectile.begin(); iter != m_Projectile.end(); iter++)
	{
		for (auto src = m_pNpcList.begin(); src != m_pNpcList.end();)
		{
			TRect inst;
			if (TCollision::RectToRect((*iter)->m_rtCollision, (*src)->m_rtCollision, inst))
			{
				AddEffect(inst.cx, inst.cy);
				(*src)->Release();
				delete* src;
				src = m_pNpcList.erase(src);
			}
			else
			{
				src++;
			}
		}
	}

	for (auto src = m_pNpcList.begin(); src != m_pNpcList.end();)
	{
		TRect inst;
		if (TCollision::RectToRect((*src)->m_rtCollision, m_pUser->m_rtCollision, inst))
		{
			AddEffect(inst.cx, inst.cy);
			(*src)->Release();
			delete* src;
			src = m_pNpcList.erase(src);
			continue;
		}
		bool bFlag = false;
		for (auto dest = m_pNpcList.begin(); dest != m_pNpcList.end(); )
		{
			TRect npcinst;
			if (src == dest)
			{
				dest++;
				continue;
			}
			if (TCollision::RectToRect((*src)->m_rtCollision, (*dest)->m_rtCollision, npcinst))
			{
				AddEffect(npcinst.cx, npcinst.cy);
				(*dest)->Release();
				delete* dest;
				bFlag = true;
				dest = m_pNpcList.erase(dest);
			}
			else
			{
				dest++;
			}
		}
		if (bFlag)
		{
			(*src)->Release();
			delete* src;
			src = m_pNpcList.erase(src);
		}
		else
		{
			src++;
		}
	}

	for (auto iter = m_pEffectList.begin();
		iter != m_pEffectList.end(); )
	{
		TEffect2D* pEffect = *iter;
		if (pEffect->Update() == false)
		{
			delete pEffect;
			iter = m_pEffectList.erase(iter);
			continue;
		}
		iter++;
	}
	return true;
}
bool TSceneInGame::Render()
{
	m_pMap->Render();

	for (auto data : m_pNpcList)
	{
		data->Render();
	}

	m_pUser->PreRender();
	m_pImmediateContext->PSSetShaderResources(1, 1,
		&m_pUser->m_pMaskTex->m_pTextureSRV);
	m_pUser->PostRender();


	for (auto pEffect : m_pEffectList)
	{
		pEffect->m_pSprite->SetCameraSize(vSize);
		pEffect->m_pSprite->SetCameraPos(m_vCamera);
		pEffect->m_pSprite->SetRect(pEffect->m_tRect);
		pEffect->m_pSprite->SetPosition(pEffect->m_vPos, m_vCamera);
		pEffect->m_pSprite->PreRender();
		m_pImmediateContext->PSSetShaderResources(1, 1,
			&pEffect->m_pSprite->m_pMaskTex->m_pTextureSRV);
		pEffect->m_pSprite->PostRender();
	}

	for (auto projectile : m_Projectile)
	{
		projectile->m_pSprite->SetCameraSize(vSize);
		projectile->m_pSprite->SetCameraPos(m_vCamera);
		projectile->m_pSprite->SetRect(projectile->m_tRect);
		projectile->m_pSprite->SetPosition(projectile->m_vPos, m_vCamera);
		projectile->m_pSprite->PreRender();
		m_pImmediateContext->PSSetShaderResources(1, 1,
			&projectile->m_pSprite->m_pMaskTex->m_pTextureSRV);
		projectile->m_pSprite->PostRender();
	}

	//DrawMiniMap(0, 0);
	return true;
}
void TSceneInGame::DrawMiniMap(UINT x, UINT y, UINT w, UINT h)
{
	D3D11_VIEWPORT saveViewPort[15];
	UINT SaveNumView=1;
	m_pImmediateContext->RSGetViewports(&SaveNumView, saveViewPort);

	D3D11_VIEWPORT vp;
	vp.Width = w;
	vp.Height = h;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);
	//TVector2 vCamera = { 0,0 };
	TVector2 vSize = { 2000, 2000 };
	m_pMap->SetCameraSize(vSize);
	m_pMap->SetCameraPos(m_vCamera);
	m_pMap->Frame();
	m_pMap->Render();
	for (auto npc : m_pNpcList)
	{
		npc->SetCameraSize(vSize);
		npc->SetCameraPos(m_vCamera);
		npc->Frame();
		npc->Render();
	}
	m_pImmediateContext->RSSetViewports(SaveNumView, saveViewPort);
}
bool TSceneInGame::Release()
{
	m_pMap->Release();
	delete m_pMap;
	m_pMap = nullptr;

	m_pUser->Release();
	delete m_pUser;
	m_pUser = nullptr;

	for (auto data : m_pNpcList)
	{
		data->Release();
		delete data;
	}

	for (auto data : m_pEffectList)
	{
		delete data;
	}
	m_pEffectList.clear();

	for (auto data : m_Projectile)
	{
		delete data;
	}
	m_Projectile.clear();
	return true;
}