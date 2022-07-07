#include "TIntroWorld.h"
#include "TInput.h"
#include <string>
#include "TObjectMgr.h"
#include "TUIModelMgr.h"
// 1,인터페이스 설계( 인트로, 로비, 인게임, 결과 등)
// 2,게임시스템(룰) : 엔딩조건, 아이템, 케릭터, 맵, 어떤게임을 모작
// 스케줄.. 
// 3,파워포인트로 제출(기획(설계,복안)이번주 목요일)
// 4,3월14일 월요일 제출

bool	TIntroWorld::CreateModelType()
{
	/// <summary>
	/// 배경이미지
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	TShader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.txt", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	std::shared_ptr<TImageObject> obj(new TImageObject);
	obj->m_csName = L"TImageObject:bk";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(TVector2(0, 0));
	obj->m_pColorTex = m_pColorTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(TCollisionType::Ignore, TSelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bk", obj));

	/// <summary>
	/// 대화상자
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	std::shared_ptr<TButtonObject> btnDlg(new TButtonObject);
	btnDlg->m_csName = L"TButtonObject:btnDlg";
	btnDlg->Init();
	btnDlg->m_rtOffset = { 50, 50, 50, 50 };
	btnDlg->SetRectDraw({ 0,0, g_rtClient.right / 3,g_rtClient.bottom / 3 });
	btnDlg->SetPosition(TVector2(0, 0));
	if (!btnDlg->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/ui/popup_normal.png"))
	{
		return false;
	}
	btnDlg->SetCollisionType(TCollisionType::Ignore, TSelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnDlg", btnDlg));



	/// <summary>
	/// 버튼
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	std::shared_ptr<TButtonObject> btnObj(new TButtonObject);
	btnObj->m_csName = L"TButtonObject:btnStart";
	btnObj->Init();
	btnObj->m_rtOffset = { 0, 0, 0, 0 };
	btnObj->SetRectDraw({ 0,0, 334,82 });
	btnObj->SetPosition(TVector2(0, 0));
	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TSound* pSound = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_pus.png");
	pSound = I_Sound.Load("../../data/Sound/FootStepSound.wav");
	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_sel.png");
	pSound = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");
	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_dis.png");
	pSound = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");
	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);

	if (!btnObj->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/main_start_nor.png"))
	{
		return false;
	}
	btnObj->SetCollisionType(TCollisionType::Ignore, TSelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnStart", btnObj));

	// 새로운 모델을 생성해서 등록한다.
	std::shared_ptr<TUIModelComposed> pComposedObj(new TUIModelComposed);
	pComposedObj->m_csName = L"TUIModelComposed";
	TButtonObject* pDlgWindow = (TButtonObject*)I_UI.GetPtr(L"btnDlg")->Clone();
	pDlgWindow->m_rtOffset = { 50, 50, 50, 50 };
	pDlgWindow->SetRectDraw({ 0,0, g_rtClient.right / 3,g_rtClient.bottom / 3 });
	pDlgWindow->SetPosition(TVector2(400, 300));
	pComposedObj->Add(pDlgWindow);
	TUIModel* pNewDlgBtn = I_UI.GetPtr(L"btnStart")->Clone();// new TButtonObject(*I_UI.GetPtr(L"btnStart"));
	pNewDlgBtn->m_csName = L"btnStartClone_ComposedList";
	pNewDlgBtn->SetRectDraw({ 0,0, 100,50 });
	pNewDlgBtn->SetPosition(pDlgWindow->m_vPos+TVector2(0, 0));
	pComposedObj->Add(pNewDlgBtn);
	I_UI.m_list.insert(std::make_pair(L"dlgWindow", pComposedObj));
	return true;
}
bool	TIntroWorld::Init()
{
	TWorld::Init();
	//m_pBackGroundMusic->Play(true);
	return true;
}
bool	TIntroWorld::Load(std::wstring file)
{	
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");	
	m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");

	for (int i = 0; i < 10; i++)
	{
		std::wstring name = L"../../data/";
		name += std::to_wstring(i);
		name +=L".bmp";
		m_pChangeColorTex[i] = I_Texture.Load(name);
	}	
	CreateModelType();
	
	TObject2D* pNewBK =
		I_UI.GetPtr(L"bk")->Clone();
		pNewBK->m_csName = L"TImageObjectClock:bk";
		pNewBK->SetPosition(TVector2(400, 300));
		pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<TObject2D>(pNewBK));
	// 프로토타입 디자인 패턴-> 복제를 통해서 객체 생성/ + 컴포짓(Composite패턴)
	TObject2D* pNewBtn1 =
		I_UI.GetPtr(L"btnStart")->Clone();
		pNewBtn1->m_csName = L"btnStartClone1";
		pNewBtn1->SetRectDraw({ 0,0, 100,50 });
		pNewBtn1->SetPosition(TVector2(300, 25));
		pNewBtn1->UpdateData();
	m_UIObj.push_back(std::shared_ptr<TObject2D>(pNewBtn1));
	TUIModel* pNewBtn2 =I_UI.GetPtr(L"btnStart")->Clone();
		pNewBtn2->m_csName = L"btnStartClone2";
		pNewBtn2->SetRectDraw({ 0,0, 100,100 });
		pNewBtn2->SetPosition(TVector2(400, 150));
		pNewBtn2->UpdateData();
	m_UIObj.push_back(std::shared_ptr<TObject2D>(pNewBtn2));
	
	TUIModel*  pNewBtn3 =I_UI.GetPtr(L"btnStart")->Clone();
		pNewBtn3->m_csName = L"btnStartClone3";
		pNewBtn3->SetRectDraw({ 0,0, 100,50 });
		pNewBtn3->SetPosition(TVector2(500, 200));
		pNewBtn3->UpdateData();
	m_UIObj.push_back(std::shared_ptr<TObject2D>(pNewBtn3));
	
	TUIModel* pNewDlgBtnClone = I_UI.GetPtr(L"dlgWindow")->Clone();
		pNewDlgBtnClone->m_csName = L"TUIModelComposedClone";
		pNewDlgBtnClone->m_pParent = nullptr;
		pNewDlgBtnClone->SetPosition(TVector2(0, 0));
		pNewDlgBtnClone->UpdateData();
	m_UIObj.push_back(std::shared_ptr<TObject2D>(pNewDlgBtnClone));


	std::shared_ptr<TListCtrlObject> pListCtrl =
		std::make_shared<TListCtrlObject>();	
	pListCtrl->m_csName = L"TListCtrlObject";
	pListCtrl->m_pParent = pNewBK;
	pListCtrl->SetRectDraw({ 100,100, 100,300 });
	//pListCtrl->SetPosition(TVector2(400, 300));
	pListCtrl->Create(1,5);	
	m_UIObj.push_back(pListCtrl);

	/// <summary>
	/// 이펙트
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	I_Sprite.Load(L"SpriteData.txt");
	m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtExplosion");

	m_ExplosionObj.Init();
	m_ExplosionObj.SetPosition(TVector2(400, 500));
	TSprite* pSprite = I_Sprite.GetPtr(L"rtExplosion");	
	m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
	m_ExplosionObj.SetRectDraw({ 0,0, 34, 46 });

	if (!m_ExplosionObj.Create(m_pd3dDevice, m_pContext,
		L"Shader.txt",
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp"))
	{
		return false;
	}
	return true;
}
bool	TIntroWorld::Frame()
{	
	if (m_bLoadZone && m_pNextWorld!=nullptr)
	{
		I_ObjectMgr.Release();
		m_pNextWorld->Load(L"zone.txt");
		TWorld::m_pWorld = m_pNextWorld;
	}
	m_pBackGroundMusic->Frame();
	static int iIndex = 0;
	//if ( TInput::Get().GetKey(VK_F2) == KEY_PUSH)
	//{
	//	iIndex++;
	//	if (iIndex >= 10)
	//	{
	//		iIndex = 0;
	//	}
	//	auto data = m_UIObj.find(L"bk");
	//	if (data != m_UIObj.end())
	//	{
	//		data->second->m_pColorTex = m_pChangeColorTex[iIndex];			
	//	}		
	//}
	if (TInput::Get().GetKey(VK_F3) == KEY_PUSH)
	{
		iIndex++;
		if (iIndex >= m_ExplosionObj.m_pSprite->m_rtArray.size())
		{
			iIndex = 0;
		}
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[iIndex]);
			m_ExplosionObj.SetRectDraw({ 0,0,
				m_ExplosionObj.m_pSprite->m_rtArray[iIndex].right,
				m_ExplosionObj.m_pSprite->m_rtArray[iIndex].bottom });
	}
	if (TInput::Get().GetKey(VK_F4) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtExplosion");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	if (TInput::Get().GetKey(VK_F6) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtBomb");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	if (TInput::Get().GetKey(VK_F7) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtClash");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	m_ExplosionObj.Frame();
	TWorld::Frame();
	return true;
}
bool	TIntroWorld::Render() 
{	
	TWorld::Render();
	m_ExplosionObj.Render();
	return true;
}
bool	TIntroWorld::Release() 
{	
	TWorld::Release();
	m_ExplosionObj.Release();
	return true;
}