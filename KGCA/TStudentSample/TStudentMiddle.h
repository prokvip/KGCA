#pragma once
#include "TStudent.h"
class TStudentMiddle : public TStudent
{
public:
	int			m_iKor; // 2
	int			m_iEng; // 3
	int			m_iMat; // 4
	template<typename T>
	T operator [] (int id);
	template<>
	float operator [] (int id);
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
