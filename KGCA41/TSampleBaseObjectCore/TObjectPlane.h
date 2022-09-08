#pragma once
#include "TBaseObject.h"
// 기본 : 클라이언트 영역 가득 체운다.
// 원하는 지점에 원하는 크기로 랜더링.
class TObjectPlane : public TBaseObject
{
public:
	void    CreateVertexData() override;
	void    UpdateVertexBuffer() override;
};

