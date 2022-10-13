#include "TSceneTitle.h"
bool     TSceneTitle::IsNextScene()
{
	//if (m_btnStart->m_CurrentState == UI_SELECT)
	//{
	//	return true;
	//}
	return false;
}
bool TSceneTitle::Init()
{
	I_Sprite.Load(L"SpriteInfo.txt");
	I_Sprite.Load(L"ui.txt");
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pBG = new TBaseObject;
	m_pBG->Create(m_pd3dDevice,m_pImmediateContext,shaderfilename,L"../../data/gameHeight.png");
	return true;
}
bool TSceneTitle::Frame()
{		
	TMatrix m,s,t,c;
	float fScale = cos(g_fGameTimer) * 0.5f + 0.5f;
	s = s.Scale(fScale, fScale, fScale);
	m = m.RotationZ(g_fGameTimer);
	t = t.Translation(0.5f, 0, 0);
	c = s * m * t;
	for (int i = 0; i < m_pBG->m_InitVertexList.size(); i++)
	{
		TVector v = m_pBG->m_InitVertexList[i].p;
		v = v * s; // s * r * t 		
		v = v * m; 
		v = v * t;
		m_pBG->m_VertexList[i].p = v;
	}
	m_pBG->UpdateVertexBuffer();
	return true;
}
bool TSceneTitle::Render()
{
	m_pBG->Render();	
	return true;
}
bool TSceneTitle::Release()
{
	m_pBG->Release();	
	return true;
}