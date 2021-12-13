#pragma once
#include "TObject.h"
class TStudent : public TObject
{
public:
	int			m_iIndex;
	int			m_iTotal;
	float		m_fAverage;	
public:
	// 가상테이블 생성됨.
	virtual void	Show();
	virtual void	Save();
	virtual void	Load() override;
	virtual void    SetData(int iIndex);
	int& operator [] (int id);
	// p1 * 3 , 3 * p1
	friend std::ostream& operator << (
		std::ostream& os,
		TStudent& data);
	TStudent()
	{
		m_iIndex = 0;
		m_iType  = 0;
	}
	TStudent(int i, int kor) 
	{
		m_iIndex = i;
		m_iType = 0;
	}
	virtual ~TStudent()
	{
		int k = 0;
	}
};

