#include "TSceneTitle.h"
#include "TInput.h"
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
	//camera
	TMatrix matView;
	static TVector vPos = { 0,0,-10 };
	static TVector vTarget = { 0,0,10 };
	if (I_Input.GetKey('W') == KEY_HOLD)
	{
		vPos.z += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('S') == KEY_HOLD)
	{
		vPos.z -= 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		vTarget.x += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		vTarget.x -= 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('Q') == KEY_HOLD)
	{
		vTarget.y += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('E') == KEY_HOLD)
	{
		vTarget.y -= 10.0f * g_fSecondPerFrame;
	}
	
	TVector vUp = { 0,1,0 };
	matView.ViewLookAt(vPos, vTarget, vUp);

	TMatrix matProj;
	matProj.PerspectiveFovLH(1.0f, 100.0f, 3.141592f*0.5f, 800.0f/600.0f);

	TMatrix m,s,t,c;
	float fScale = cos(g_fGameTimer) * 0.5f + 0.5f;
	s = s.Scale(10, 10, 10);
	//m = m.RotationZ(g_fGameTimer);
	t = t.Translation(0.0f, 0, 0);
	c = s * m * t;
	for (int i = 0; i < m_pBG->m_InitVertexList.size(); i++)
	{
		TVector v = m_pBG->m_InitVertexList[i].p;
		//v = v * s; // s * r * t 		
		//v = v * m; 
		//v = v * t;
		// workd transform
		TVector vWorld = v * c;
		TVector vView  = vWorld * matView;
		TVector vProj  = vView * matProj;
		vProj.x /= vProj.z;
		vProj.y /= vProj.z;
		vProj.z /= vProj.z;
		m_pBG->m_VertexList[i].p = vProj;
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