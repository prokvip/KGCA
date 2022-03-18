#include "TObject3D.h"
void		TObject3D::SetMatrix(TMatrix* matWorld,
	TMatrix* matView, TMatrix* matProj)
{
	m_ConstantList.matWorld = m_matWorld.Transpose();
	if (matWorld != nullptr)
	{
		m_ConstantList.matWorld = matWorld->Transpose();
	}
	if (matView != nullptr)
	{
		m_ConstantList.matView = matView->Transpose();
	}
	if (matProj != nullptr)
	{
		m_ConstantList.matProj = matProj->Transpose();
	}
}
void		TObject3D::AddPosition(TVector3 vPos)
{
	// ������ġ
	m_vPos += vPos;
	//  10,10f    ~     w(50, 10)
	//
	//   h
	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(
			m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	}
}
/// <summary>
/// m_rtDraw, m_rtCollision ���ŵȴ�.
/// </summary>
/// <param name="vPos"></param>
void		TObject3D::SetPosition(TVector3 vPos)
{
	m_vPos = vPos;
	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;
}
bool    TObject3D::SetVertexData()
{
	return true;
}
bool    TObject3D::SetIndexData()
{
	m_IndexList.clear();
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
void    TObject3D::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}
void    TObject3D::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
bool	TObject3D::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_ConstantList.Color = m_vColor;
	m_ConstantList.Timer = TVector4(
		g_fGameTimer,
		0,
		0,
		1.0f);	
	return true;
}
TObject3D::TObject3D()
{
	m_fAlpha = 1.0f;
	m_vColor = TVector4(1, 1, 1, 1);
}
TObject3D::~TObject3D()
{

}