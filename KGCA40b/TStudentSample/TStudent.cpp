#include "TStudent.h"

template<typename T>
T TStudent::operator [] (int id)
{
	if (id == 0) return m_iIndex;
	if (id == 1) return m_iType;
	if (id == 6) return m_iTotal;
	return -1;
}
template<>
float TStudent::operator [] (int id)
{
	if (id == 6) return m_fAverage;
	return -1.0f;
}

void TStudent::SetData(int iIndex)
{
	m_iIndex = iIndex;
	m_iTotal = 0;
	strcpy(m_csName, "none");
	m_fAverage = m_iTotal / 3.0f;
}

std::ostream& operator << (
	std::ostream& os,
	TStudent& data)
{
	os  << data.m_iIndex << " "
		<< data.m_iTotal << " "
		<< data.m_fAverage << std::endl;
	return os;
}
void		TStudent::Show()
{
	std::cout  << operator[]<int>(0) << " "
		<< operator[]<int>(5) << " "
		<< operator[]<float>(6) << " ";
}
void		TStudent::Save()
{	
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iIndex, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iType, sizeof(int));
	m_iCurrentPosition += sizeof(int);
}
/*
*	Load
*/
void		TStudent::Load()
{
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iIndex, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iType, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);
}


