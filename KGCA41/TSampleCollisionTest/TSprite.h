#pragma once
#include "TObject2D.h"
typedef std::vector<RECT>  RECT_ARRAY;
class TSprite : public TObject2D
{
public:
	W_STR			m_szName;
	UINT			m_IIndex;
public:
	RECT_ARRAY		m_uvArray;
	std::vector<TSprite*>  m_pChild;
	TRect    m_rtCollision;
public:
	HRESULT Load(ID3D11Device* pd3dDevice,// µð¹ÙÀÌ½º °´Ã¼
		ID3D11DeviceContext* pImmediateContext, std::wstring name);
	//// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
	//void  SetPosition(TVector2D vPos)
	//{
	//	m_vPos = vPos;
	//	TVector2D	vDrawSize;
	//	vDrawSize.x = m_rtInit.w / 2.0f;
	//	vDrawSize.y = m_rtInit.h / 2.0f;
	//	m_rtCollision.Set(
	//		vPos.x - vDrawSize.x,
	//		vPos.y - vDrawSize.y,
	//		m_rtInit.w,
	//		m_rtInit.h);

	//	SetPosition(vPos, m_vCameraPos);
	//	//// 0  ~ 800   -> 0~1 ->  -1 ~ +1
	//	//m_vDrawPos.x = (m_rtCollision.x1 / g_rtClient.right) * 2.0f - 1.0f;
	//	//m_vDrawPos.y = -((m_rtCollision.y1 / g_rtClient.bottom) * 2.0f - 1.0f);
	//	//m_vDrawSize.x = (m_rtInit.w / g_rtClient.right) * 2;
	//	//m_vDrawSize.y = (m_rtInit.h / g_rtClient.bottom) * 2;
	//	//UpdateVertexBuffer();
	//}
};

