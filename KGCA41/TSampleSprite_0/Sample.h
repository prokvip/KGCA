#pragma once
#include "TGameCore.h"
#include "TBaseObject.h"

class TUser2D : public TBaseObject
{
public:
	POINT	    m_ptImageSize;
	TRect		m_rtInit;
	TRect		m_rtUV;
	TVector2D	m_vPos;
private:
	TVector2D	m_vDrawPos;
	TVector2D	m_vDrawSize;
public:
	bool  Frame() override;
	void  SetRect(TRect vPos);
	void  SetPosition(TVector2D vPos);
	void  UpdateVertexBuffer();
};
class Sample : public TGameCore
{
	TUser2D*					  m_pUser;
	std::vector<TBaseObject*> m_pObjectList;
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

