#include "Sample.h"

class TTest
{
public:
	int k;
public:
	TTest()
	{
		k = 10;
	}
	~TTest()
	{
		k = 0;
	}
};
//std::shared_ptr<TTest>   pTitle = nullptr;
void Function(std::shared_ptr<TTest> data)
{
	data.get()->k = 20;
}
void Function(std::unique_ptr<TTest> data)
{
	data.get()->k = 20;
}
bool Sample::Init()
{
	//TTest* title = new TTest;
	//delete title;
	//title = nullptr;
	//if (title) delete title;

	// 참조카운팅 가능
	{
		std::shared_ptr<TTest>  pTitleShared = std::make_shared<TTest>();
		{
			TTest* pData = pTitleShared.get();		
			Function(pTitleShared);
		}
	}
	// 참조카운팅 불가
	{
		std::unique_ptr<TTest>  pTitle = std::make_unique<TTest>();
		//std::unique_ptr<TTest> pNewData = std::move(pTitle);
		//std::unique_ptr<TTest>  pTitle2 = pTitle; 
		{
			TTest* pData = pTitle.get();			
		/*	TTest* pData2 = pTitle.get();
			delete pData2;*/
			//Function(pTitle);
		}
	}

	m_pTitle = std::make_shared<TSceneTitle>();
	m_pInGame = std::make_shared<TSceneInGame>(); 
	m_pTitle->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pInGame->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"");
	m_pTitle->Init();
	m_pInGame->Init();
	m_pCurrentScene = m_pTitle;
	return true;
}
bool Sample::Frame()
{		
	if (m_pCurrentScene->IsNextScene())
	{
		m_pCurrentScene = m_pInGame;
	}
	m_pCurrentScene->Frame();
	return true;
}
bool Sample::Render()
{	
	if (I_Input.GetKey('V') == KEY_HOLD)
	{
		m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSWireFrame);
	}
	m_pCurrentScene->Render();	
	return true;
}
bool Sample::Release()
{	
	m_pTitle->Release();
	m_pInGame->Release();
	return true;
}

GAME_RUN(TSampleCollisionTest, 800,600)