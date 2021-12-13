#pragma once
#include "TStudent.h"
class TStudentHigh : public TStudent
{
public:
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
public:
	void		Show() override;
	void		Save() override;
	void		Load() override;
	void		SetData(int iIndex)override;
public:
	TStudentHigh()
	{
		m_iType = 2;
	}
	TStudentHigh(int i, int kor)
	{
		m_iIndex = i;
		m_iKor = kor;
		m_iType = 2;
	}
	~TStudentHigh()
	{
		int k = 0;
	}
};
