#pragma once
#include "TDevice.h"
#include "TBaseObject.h"
#include "TGameCore.cpp"
class Sample : public TDevice
{
	TGameCore<TObject>	game3D;
	TGameCore<TObject2D>game2D;
	TBaseObject		m_object;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

