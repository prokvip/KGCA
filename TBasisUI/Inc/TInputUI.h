#pragma once
#include "TBasisUIStd.h"
namespace TUI
{
	enum KeyState
	{
		KEY_FREE = 0,
		KEY_UP,
		KEY_PUSH,
		KEY_HOLD,
	};
	class TBASISUI_API TInput : public TSingleton<TInput>
	{
	private:
		DWORD		m_dwKeyState[256]; // mouse ��ư + Ű��ư
	public:
		POINT		m_ptPos; // mouse position
	public:
		virtual bool		Init(); // �ʱ�ȭ
		virtual bool		Frame();// �ǽð� ���
		virtual bool		Render();// �ǽð� ������
		virtual bool		Release();// �Ҹ� �� ����
	public:
		DWORD  GetKey(DWORD dwKey);
	};
#define I_InputUI TInput::GetInstance()
};