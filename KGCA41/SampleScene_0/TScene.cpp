#include "TScene.h"
#include "ICoreStd.h"
#include "TInput.h"
void TScene::AddSelectExecute(TObject* owner, 
	SelectFunction func)
{
	owner->m_iSelectID = m_iExecuteSelectID++;
	m_SelectObjectList.insert(
		std::make_pair(owner->m_iSelectID, owner));
	m_fnSelectExecute.insert(
		std::make_pair( owner->m_iSelectID, func));
}
void TScene::DeleteSelectExecute(int iID)
{
	auto iter1 = m_SelectObjectList.find(iID);
	if (iter1 != m_SelectObjectList.end())
	{
		m_SelectObjectList.erase(iter1);
	}
	auto iter2 = m_fnSelectExecute.find(iID);
	if (iter2 != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(iter2);
	}
}
bool TScene::Init()
{
	m_pMapObj = std::make_unique<TBackgoundObj>();
	m_pMapObj->Set(ICore::g_pDevice, ICore::g_pContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(m_szBackround, L"../../res/shader/Plane.hlsl");
	
	TVector2 rt = { m_pMapObj->m_vPos.x, m_pMapObj->m_vPos.y };
	m_pMapObj->SetRect(rt, m_pMapObj->m_vScale.x * 2.0f, m_pMapObj->m_vScale.y * 2.0f);

	AddSelectExecute(m_pMapObj.get(),
		std::bind(&TObject::MouseOverlap, m_pMapObj.get(),
							std::placeholders::_1,
							std::placeholders::_2));
	return true;
}
bool TScene::Frame()
{
	for (auto obj : m_SelectObjectList)
	{
		TObject* pObj = obj.second;
		TVector3 mouse = I_Input.GetWorldPos(
			{ (float)g_dwWindowWidth , (float)g_dwWindowHeight },
			ICore::g_pMainCamera->m_vCameraPos);
		TVector2 pt = { mouse.x, mouse.y };

		auto fnFunction = m_fnSelectExecute.find(pObj->m_iSelectID);
		if (pObj->m_tRT.ToPoint(pt))
		{			
			if (fnFunction != m_fnSelectExecute.end())
			{
				SelectFunction call = fnFunction->second;
				call(pt, TSelectState::T_HOVER);
			}
			if (I_Input.m_dwKeyState[VK_LBUTTON] == KEY_PUSH)
			{
				SelectFunction call = fnFunction->second;
				call(pt, TSelectState::T_ACTIVE);
			}
			if (I_Input.m_dwKeyState[VK_LBUTTON] == KEY_UP)
			{
				SelectFunction call = fnFunction->second;
				call(pt, TSelectState::T_SELECTED);
			}

		}
		else
		{
			//if (pObj->m_iSelectState != TSelectState::T_DEFAULT)
			{
				SelectFunction call = fnFunction->second;
				call(pt, TSelectState::T_DEFAULT);
			}
		}
	}
	m_pMapObj->Frame();
	return true;
}
bool TScene::Render()
{
	m_pMapObj->SetMatrix(nullptr,
		&ICore::g_pMainCamera->m_matView,
		&ICore::g_pMainCamera->m_matOrthoProjection);
	m_pMapObj->Render();
	return true;
}
bool TScene::Release()
{
	m_pMapObj->Release();

	DeleteSelectExecute(m_pMapObj->m_iSelectID);
	return true;
}
void  TScene::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{

}
bool TSceneTitle::Init()
{
	TScene::Init();
	return true;
}
bool TSceneTitle::Frame()
{
	TScene::Frame();
	return true;
}
bool TSceneTitle::Render()
{	
	TScene::Render();
	return true;
}
bool TSceneTitle::Release()
{
	TScene::Release();
	return true;
}