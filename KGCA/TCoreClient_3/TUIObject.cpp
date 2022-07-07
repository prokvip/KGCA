#include "TUIObject.h"
#include "TWorld.h"
#include "TSoundMgr.h"
bool    TUIObject::SetVertexData()
{
	//ConvertIndex(m_vPos, m_fWidth, m_fHeight, m_VertexList);
	// 0  a   1     2  b   3
	// c  lt          rt
	// 4     5      6     7
	//              
	// 8     9     10    11
	// d lb           rb 
	// 12    13    14    15 
	TVector2 vLT = { (float)m_rtOffset.left,  (float)m_rtOffset.top };
	TVector2 vRT = { (float)m_rtOffset.right, (float)m_rtOffset.top };
	TVector2 vLB = { (float)m_rtOffset.left, (float)m_rtOffset.bottom };
	TVector2 vRB = { (float)m_rtOffset.right, (float)m_rtOffset.bottom };

	float fWidth = m_pColorTex->m_TextureDesc.Width;
	float fHeight = m_pColorTex->m_TextureDesc.Height;
	TVector2 tLT = { vLT.x / fWidth, vLT.y / fHeight };
	TVector2 tRT = { vRT.x / fWidth, vRT.y / fHeight };
	TVector2 tLB = { vLB.x / fWidth, vLB.y / fHeight };
	TVector2 tRB = { vRT.x / fWidth, vRT.y / fHeight };	


	std::vector<SimpleVertex> list(16);
	float halfWidth = m_fWidth / 2.0f;
	float halfHeight = m_fHeight / 2.0f;
	list[0].v = { m_vPos.x - halfWidth, m_vPos.y - halfHeight };
	list[0].t = { (float)m_rtOffsetTex.left,(float)m_rtOffsetTex.top };
	list[3].v = { m_vPos.x + halfWidth, m_vPos.y - halfHeight };
	list[3].t = { (float)m_rtOffsetTex.right,(float)m_rtOffsetTex.top };
	list[12].v = { m_vPos.x - halfWidth, m_vPos.y + halfHeight };
	list[12].t = { (float)m_rtOffsetTex.left,(float)m_rtOffsetTex.bottom };
	list[15].v = { m_vPos.x + halfWidth, m_vPos.y + halfHeight };
	list[15].t = { (float)m_rtOffsetTex.right,(float)m_rtOffsetTex.bottom };
	list[5].v = { list[0].v.x + vLT.x, list[0].v.y + vLT.y };
	list[5].t = { list[0].t.x + tLT.x, list[0].t.y + tLT.y };
	list[6].v = { list[3].v.x - vRT.x, list[3].v.y + vRT.y };
	list[6].t = { list[3].t.x - tRT.x, list[3].t.y + tRT.y };
	list[9].v = { list[12].v.x + vLB.x, list[12].v.y - vLB.y };
	list[9].t = { list[12].t.x + tLB.x, list[12].t.y - tLB.y };
	list[10].v = { list[15].v.x - vRB.x, list[15].v.y - vRB.y };
	list[10].t = { list[15].t.x - tRB.x, list[15].t.y - tRB.y };
	
	list[1].v = { list[5].v.x, list[0].v.y };
	list[1].t = { list[5].t.x, list[0].t.y };
	list[2].v = { list[6].v.x, list[3].v.y };
	list[2].t = { list[6].t.x, list[3].t.y };

	list[4].v = { list[0].v.x, list[5].v.y };
	list[4].t = { list[0].t.x, list[5].t.y };
	list[7].v = { list[3].v.x, list[6].v.y };
	list[7].t = { list[3].t.x, list[6].t.y };

	list[8].v = { list[12].v.x, list[9].v.y };
	list[8].t = { list[12].t.x, list[9].t.y };
	list[11].v = { list[15].v.x, list[10].v.y };
	list[11].t = { list[15].t.x, list[10].t.y };

	list[13].v = { list[9].v.x, list[12].v.y };
	list[13].t = { list[9].t.x, list[12].t.y };
	list[14].v = { list[10].v.x, list[15].v.y };
	list[14].t = { list[10].t.x, list[15].t.y };

	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		list[i].v.x = list[i].v.x / g_rtClient.right;
		list[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		list[i].v.x = list[i].v.x * 2.0f - 1.0f;
		list[i].v.y = -1.0f * (list[i].v.y * 2.0f - 1.0f);
	}
	m_VertexList = list;
	return true;
}
bool    TUIObject::SetIndexData()
{
	// 0  a   1     2  b   3
	// c  lt          rt
	// 4     5      6     7
	//              
	// 8     9     10    11
	// d lb           rb 
	// 12    13    14    15 
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(4);
	m_IndexList.push_back(4); m_IndexList.push_back(1); m_IndexList.push_back(5);
	m_IndexList.push_back(1); m_IndexList.push_back(2); m_IndexList.push_back(5);
	m_IndexList.push_back(5); m_IndexList.push_back(2); m_IndexList.push_back(6);
	m_IndexList.push_back(2); m_IndexList.push_back(3); m_IndexList.push_back(6);
	m_IndexList.push_back(6); m_IndexList.push_back(3); m_IndexList.push_back(7);

	m_IndexList.push_back(4); m_IndexList.push_back(5); m_IndexList.push_back(8);
	m_IndexList.push_back(8); m_IndexList.push_back(5); m_IndexList.push_back(9);
	m_IndexList.push_back(5); m_IndexList.push_back(6); m_IndexList.push_back(9);
	m_IndexList.push_back(9); m_IndexList.push_back(6); m_IndexList.push_back(10);
	m_IndexList.push_back(6); m_IndexList.push_back(7); m_IndexList.push_back(10);
	m_IndexList.push_back(10); m_IndexList.push_back(7); m_IndexList.push_back(11);


	m_IndexList.push_back(8); m_IndexList.push_back(9); m_IndexList.push_back(12);
	m_IndexList.push_back(12); m_IndexList.push_back(9); m_IndexList.push_back(13);
	m_IndexList.push_back(9); m_IndexList.push_back(10); m_IndexList.push_back(13);
	m_IndexList.push_back(13); m_IndexList.push_back(10); m_IndexList.push_back(14);
	m_IndexList.push_back(10); m_IndexList.push_back(11); m_IndexList.push_back(14);
	m_IndexList.push_back(14); m_IndexList.push_back(11); m_IndexList.push_back(15);

	return true;
}
bool	TUIObject::Frame()
{
	TObject2D::Frame();
	return true;
}
bool	TUIObject::Render()
{
	TObject2D::Render();
	return true;
}
bool	TImageObject::Init()
{
	m_bFadeIn = true;
	return true;
}
bool	TImageObject::Frame()
{
	m_vColor.x = m_fAlpha;
	m_vColor.y = m_fAlpha;
	m_vColor.z = m_fAlpha;
	m_vColor.w = 1.0f;
	TObject2D::Frame();	
	return true;
}
bool	TImageObject::Render()
{
	TObject2D::Render();
	return true;
}
bool	TButtonObject::Init()
{
	//m_bFadeIn = true;
	return true;
}
bool	TButtonObject::Frame()
{
	if (m_dwSelectState == TSelectState::T_DEFAULT)
	{
		//m_pColorTex = m_pStatePlayList[0].pTex;
		//m_dwPreSelectState = m_dwSelectState;
	}
	TObject2D::Frame();
	return true;
}
bool	TButtonObject::Render()
{
	TObject2D::Render();
	return true;
}
void TButtonObject::HitSelect(TBaseObject* pObj, DWORD dwState)
{
	std::string state;
	if (m_dwPreSelectState == m_dwSelectState)
	{
		return;
	}
	if (m_pStatePlayList.size() <= 0) return;

	switch (m_dwSelectState)
	{
		case TSelectState::T_SELECTED: 
		{
			if (m_pStatePlayList[3].pTex == nullptr) break;
			m_pColorTex = m_pStatePlayList[3].pTex;
			m_pStatePlayList[3].pSound->PlayEffect();
			TWorld::m_pWorld->m_bLoadZone = true;
			state += "T_SELECTED\n";
		}break;
		case TSelectState::T_HOVER: 
		{
			if (m_pStatePlayList[1].pTex == nullptr) break;
			m_pColorTex = m_pStatePlayList[1].pTex;
			m_pStatePlayList[1].pSound->PlayEffect();

			//state += "T_HOVER\n";
		}break;
		case TSelectState::T_ACTIVE:
		{
			if (m_pStatePlayList[2].pTex == nullptr) break;
			m_pColorTex = m_pStatePlayList[2].pTex;
			m_pStatePlayList[2].pSound->PlayEffect();
			state += "T_ACTIVE\n";
		}break;
		case TSelectState::T_FOCUS: 
		{
			if (m_pStatePlayList[2].pTex == nullptr) break;
			m_pColorTex = m_pStatePlayList[2].pTex;
			m_pStatePlayList[2].pSound->PlayEffect();
			//state += "T_FOCUS\n";
		}break;
		default:
		{
			state += std::to_string(m_dwSelectState);
		}
	}	
	m_dwPreSelectState = m_dwSelectState;
	DisplayText(state.c_str());
}