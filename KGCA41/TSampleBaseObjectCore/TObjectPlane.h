#pragma once
#include "TBaseObject.h"
// �⺻ : Ŭ���̾�Ʈ ���� ���� ü���.
// ���ϴ� ������ ���ϴ� ũ��� ������.
class TObjectPlane : public TBaseObject
{
public:
	void    CreateVertexData() override;
	void    UpdateVertexBuffer() override;
};

