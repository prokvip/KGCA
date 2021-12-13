#pragma once
#include "TStudent.h"
class TStudentMiddle : public TStudent
{
public:
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
public:
	virtual void	Show()override;
	virtual void	Save()override;
	virtual void	Load() override;
	virtual void    SetData(int iIndex) override;;
public:
	TStudentMiddle()
	{
		m_iType = 1;
	}
	TStudentMiddle(int i, int kor)
	{
		m_iType = 1;
		m_iIndex = i;
		m_iKor = kor;
	}
	~TStudentMiddle()
	{
		int k = 0;
	}
};
