#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream> // std::cout, std::in
#include <memory.h>
#include <stdlib.h>
class TObject
{
public:
	int			m_iType; // 0, 1, 2
	char		m_csName[64];
	char		m_csBuffer[1000];
	int			m_iBufferSize;
	int			m_iCurrentPosition;
	virtual void	Show() = 0;// 순수가상함수
	virtual void	Save() = 0;// 순수가상함수
	virtual void	Load() = 0;// 순수가상함수
	TObject()
	{
		m_iType = -1;
		m_iBufferSize = 0;
		m_iCurrentPosition = 0;
		memset(m_csName, 0, sizeof(char) * 64);
		memset(m_csBuffer, 0, sizeof(char) * 1000);
	}
	~TObject()
	{
		m_iType = -1;
	}
};