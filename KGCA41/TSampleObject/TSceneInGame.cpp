#include "TSceneInGame.h"
bool TSceneInGame::Init()
{
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	TTexture* pMaskTex = I_Tex.Load(L"../../data/bitmap2.bmp");

	m_pMap = new TMapObject;
	m_pMap->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/kgcabk.bmp");//"L"../../data/gameHeight.png");
	m_pMap->SetRect({ 0, 0,	2000.0f,2000.0f });
	m_pMap->SetPosition({ -1000.0f, -1000.0f });
	// user character
   // { 90, 1, 40, 60 } , { 400,300 }
   // -1 ~ +1
	m_pUser = new TUser2D;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext,
		L"../../data/shader/DefaultShapeMask.txt",
		L"../../data/bitmap1.bmp");
	m_pUser->SetMask(pMaskTex);
	m_pUser->m_fSpeed = 300.0f;
	m_pUser->SetRect({ 91, 2, 39, 59 });
	m_pUser->SetPosition({ 0,0 });


	for (int iNpc = 0; iNpc < 10; iNpc++)
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
		npc->SetDirection({ randstep(-1.0f, 1.0f),
			randstep(-1.0f, 1.0f) });

		npc->SetCameraPos(m_vCamera);
		npc->SetPosition({ randstep(-400,+400), 100.0f });
		npc->SetMask(pMaskTex);
		m_pNpcList.push_back(npc);
	}
	return true;
}
bool TSceneInGame::Frame()
{
	m_pUser->SetCameraPos(m_vCamera);
	m_pUser->Frame();

	m_pMap->SetCameraPos(m_vCamera);
	m_pMap->Frame();

	for (int iObj = 0; iObj < m_pNpcList.size(); iObj++)
	{
		m_pNpcList[iObj]->SetCameraPos(m_vCamera);
		m_pNpcList[iObj]->Frame();
	}
	m_vCamera = m_pUser->m_vPos;
	return true;
}
bool TSceneInGame::Render()
{
	m_pMap->Render();
	for (int iObj = 0; iObj < m_pNpcList.size(); iObj++)
	{
		m_pNpcList[iObj]->Render();
	}

	m_pUser->PreRender();
	m_pImmediateContext->PSSetShaderResources(1, 1,
		&m_pUser->m_pMaskTex->m_pTextureSRV);
	m_pUser->PostRender();
	return true;
}
bool TSceneInGame::Release()
{
	m_pMap->Release();
	delete m_pMap;

	m_pUser->Release();
	delete m_pUser;

	for (int iObj = 0; iObj < m_pNpcList.size(); iObj++)
	{
		m_pNpcList[iObj]->Release();
		delete m_pNpcList[iObj];
	}
	return true;
}