#include "TStudent.h"
std::ostream& operator << (std::ostream& os, TStudent& data)
{
	data.Print();
	return os;
}
std::istream& operator >> (std::istream& is, TStudent& data)
{
	return is;
}

TStudent::TStudent()
{
	m_pNext = m_pPrev = nullptr;
	m_szName[0] = 0;
	m_iAge = 0;
	m_iKor = 0;
	m_iEng = 0;
	m_iMat = 0;
	m_iTotal = 0;
	m_fAverage = 0;
}
TStudent::~TStudent()
{

}