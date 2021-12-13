#pragma once
#include "TStudent.h"
class TStudentCollege : public TStudent
{
public:
	int   m_iPhysics;
public:
	void		Show() override;
	void		Save() override;
	void		Load() override;
	void		SetData(int iIndex) override;
public:
	TStudentCollege()
	{
		m_iType = 3;
	}
	TStudentCollege(int i, int kor)
	{
		m_iType = 3;
		m_iIndex = i;
		m_iPhysics = kor;
	}
	~TStudentCollege()
	{
		int k = 0;
	}
};
