#include "TStudentCollege.h"
void  TStudentCollege::SetData(int iIndex)
{
	m_iIndex = iIndex;
	strcpy(m_csName, "none");
	m_iPhysics = rand() % 100;
	m_iTotal = m_iPhysics;
	m_fAverage = m_iTotal / 3.0f;
}
void		TStudentCollege::Show()
{
	std::cout
		<< m_iIndex << " "
		<< m_iType << " "
		<< m_iPhysics << " "
		<< m_iTotal << " "
		<< m_fAverage << " " << std::endl;
}
void		TStudentCollege::Save()
{
	TStudent::Save();
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iPhysics, sizeof(int));
	m_iCurrentPosition += sizeof(int);
	m_iBufferSize = m_iCurrentPosition;
}
void		TStudentCollege::Load()
{
	TStudent::Load();
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iPhysics, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	m_iTotal = m_iPhysics;
	m_fAverage = m_iTotal / 1.0f;
}