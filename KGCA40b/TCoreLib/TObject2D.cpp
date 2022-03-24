#include "TObject2D.h"
void TObject2D::SetRectSouce(RECT rt)
{
	m_rtSource = rt;
}
void TObject2D::SetRectDraw(RECT rt)
{
	m_rtDraw = rt;
	m_vPos.x = rt.left + (rt.right  / 2.0f);
	m_vPos.y = rt.top + (rt.bottom / 2.0f);
	m_fWidth = rt.right;
	m_fHeight = rt.bottom;

	m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);	
}
void TObject2D::UpdateRectDraw(RECT rt)
{	
	m_fWidth = rt.right;
	m_fHeight = rt.bottom;
}
void		TObject2D::AddPosition(T::TVector2 vPos)
{
	// 현재위치
	m_vPos += vPos;
	//  10,10f    ~     w(50, 10)
	//
	//   h
	m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);	
	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(
			m_pVertexBuffer, 0, NULL, &m_VertexList2D.at(0), 0, 0);
	}
}
/// <summary>
/// m_rtDraw, m_rtCollision 갱신된다.
/// </summary>
/// <param name="vPos"></param>
void		TObject2D::SetPosition(T::TVector2 vPos)
{
	m_vPos = vPos;	
	/*SetRectDraw({
		(LONG)(m_vPos.x - m_rtDraw.right / 2.0f),
		(LONG)(m_vPos.y - m_rtDraw.bottom / 2.0f),
		m_rtDraw.right,
		m_rtDraw.bottom });*/
	m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);	
	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(
			m_pVertexBuffer, 0, NULL, &m_VertexList2D.at(0), 0, 0);
	}
}
void	TObject2D::Convert(
	T::TVector2 center, float fWidth, float fHeight,
	std::vector<SimpleVertex>& retList)
{
	// 0       1,4

	//     c

	// 2,3      5
	std::vector<SimpleVertex> list(6);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].v = { center.x - halfWidth, center.y - halfHeight };
	list[1].v = { center.x + halfWidth, center.y - halfHeight };
	list[2].v = { center.x - halfWidth, center.y + halfHeight };
	list[3].v = list[2].v;
	list[4].v = list[1].v;
	list[5].v = { center.x + halfWidth, center.y + halfHeight };

	Convert(list, retList);

}
void	TObject2D::Convert(
	std::vector<SimpleVertex>& list,
	std::vector<SimpleVertex>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].v.x = list[i].v.x / g_rtClient.right;
		retList[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].v.x = retList[i].v.x * 2.0f - 1.0f;
		retList[i].v.y = -1.0f * (retList[i].v.y * 2.0f - 1.0f);
	}
	// 91,1, 42, 56 => 0 ~ 1
	m_rtSource.left = 0; m_rtSource.right = 0;
	m_rtSource.top = 0; m_rtSource.bottom = 0;
	if (m_rtSource.left == 0 && m_rtSource.right == 0 &&
		m_rtSource.top == 0 && m_rtSource.bottom == 0)
	{
		retList[0].t.x = 0.0f; retList[0].t.y = 0.0f; // v0
		retList[1].t.x = 1.0f; retList[1].t.y = 0.0f; // v1
		retList[2].t.x = 0.0f; retList[2].t.y = 1.0f;
		retList[3].t = retList[2].t;
		retList[4].t = retList[1].t;
		retList[5].t.x = 1.0f; retList[5].t.y = 1.0f;
	}
	else
	{
		float u = m_rtSource.left / (float)m_TextureDesc.Width;
		float v = m_rtSource.top / (float)m_TextureDesc.Height;
		float w = m_rtSource.right / (float)m_TextureDesc.Width;
		float h = m_rtSource.bottom / (float)m_TextureDesc.Height;
		retList[0].t.x = u; retList[0].t.y = v; // v0
		retList[1].t.x = u + w; retList[1].t.y = v; // v1
		retList[2].t.x = u; retList[2].t.y = v + h;
		retList[3].t = retList[2].t;
		retList[4].t = retList[1].t;
		retList[5].t.x = u + w; retList[5].t.y = v + h;
	}
}
void	TObject2D::ConvertIndex(
	T::TVector2 center, float fWidth, float fHeight,
	std::vector<SimpleVertex>& retList)
{
	// 0       1

	//     c

	// 2       3
	std::vector<SimpleVertex> list(4);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].v = { center.x - halfWidth, center.y - halfHeight };
	list[1].v = { center.x + halfWidth, center.y - halfHeight };
	list[2].v = { center.x - halfWidth, center.y + halfHeight };
	list[3].v = { center.x + halfWidth, center.y + halfHeight };
	ConvertIndex(list, retList);

}
void	TObject2D::ConvertIndex(
	std::vector<SimpleVertex>& list,
	std::vector<SimpleVertex>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].v.x = list[i].v.x / g_rtClient.right;
		retList[i].v.y = list[i].v.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].v.x = retList[i].v.x * 2.0f - 1.0f;
		retList[i].v.y = -1.0f * (retList[i].v.y * 2.0f - 1.0f);
	}
	// 91,1, 42, 56 => 0 ~ 1	
	if (m_rtSource.left == 0 && m_rtSource.right == 0 &&
		m_rtSource.top == 0 && m_rtSource.bottom == 0)
	{
		retList[0].t.x = 0.0f; retList[0].t.y = 0.0f; // v0
		retList[1].t.x = 1.0f; retList[1].t.y = 0.0f; // v1
		retList[2].t.x = 0.0f; retList[2].t.y = 1.0f;	
		retList[3].t.x = 1.0f; retList[3].t.y = 1.0f;
	}
	else
	{
		float u = m_rtSource.left / (float)m_TextureDesc.Width;
		float v = m_rtSource.top / (float)m_TextureDesc.Height;
		float w = m_rtSource.right / (float)m_TextureDesc.Width;
		float h = m_rtSource.bottom / (float)m_TextureDesc.Height;
		retList[0].t.x = u; retList[0].t.y = v; // v0
		retList[1].t.x = u + w; retList[1].t.y = v; // v1
		retList[2].t.x = u; retList[2].t.y = v + h;	
		retList[3].t.x = u + w; retList[3].t.y = v + h;
	}
}
bool    TObject2D::SetVertexData()
{
	ConvertIndex(m_vPos, m_fWidth, m_fHeight, m_VertexList2D);	
	return true;
}
bool    TObject2D::SetIndexData()
{
	m_IndexList.clear();
	//// 0   1,4
	//// 2,3  5
	//DWORD indeces[] = {
	//	0,1,2,
	//	3,4,5,
	//};
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
void    TObject2D::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}
void    TObject2D::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
bool	TObject2D::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_ConstantList.Color = m_vColor;
	m_ConstantList.Timer = T::TVector4(
		g_fGameTimer,
		0,
		0,
		1.0f);
	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
	return true;
}
TObject2D::TObject2D()
{
	m_fAlpha = 1.0f;
	m_vColor = T::TVector4(1, 1, 1, 1);
	m_rtSource.left = 0; m_rtSource.right = 0;
	m_rtSource.top  = 0; m_rtSource.bottom = 0;
	m_rtDraw.left = 0; m_rtDraw.right = g_rtClient.right;
	m_rtDraw.top = 0; m_rtDraw.bottom = g_rtClient.bottom;
}
TObject2D::~TObject2D()
{

}