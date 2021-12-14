#include "TStudentMiddle.h"
template<typename T>
T TStudentMiddle::operator [] (int id)
{
	int iRet = TStudent::operator[]<int>(id);
	if (iRet < 0)
	{
		if (id == 2) return m_iKor;
		if (id == 3) return m_iEng;
		if (id == 4) return m_iMat;
	}
	return iRet;
}
template<>
float TStudentMiddle::operator [] (int id)
{
	int iRet = TStudent::operator[]<float>(id);
	if (iRet < 0)
	{
		if (id == 6) return m_fAverage;
	}
	return iRet;
}
void  TStudentMiddle::SetData(int iIndex)
{
	m_iIndex = iIndex;
	strcpy(m_csName, "none");
	m_iKor = rand() % 100;
	m_iEng = rand() % 100;
	m_iMat = rand() % 100;
	m_iTotal = m_iKor +
		m_iEng +
		m_iMat;
	m_fAverage = m_iTotal / 3.0f;
}
void  TStudentMiddle::Show()
{
	//t __cdecl TStudent::operator[]<int>(int)"
	// data[0] -> data.operator[]
	for (int i = 0; i < 6; i++)
	{
		std::cout
			<< operator[]<int>(i) << " ";
	}
	std::cout
		<< operator[]<float>(6) << " " << std::endl;
}
void  TStudentMiddle::Save()
{
	TStudent::Save();
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iKor, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iEng, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(pData, &m_iMat, sizeof(int));
	m_iCurrentPosition += sizeof(int);
	m_iBufferSize = m_iCurrentPosition;
}
void  TStudentMiddle::Load()
{
	TStudent::Load();
	char* pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iKor, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iEng, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	pData = &m_csBuffer[m_iCurrentPosition];
	memcpy(&m_iMat, pData, sizeof(int));
	m_iCurrentPosition += sizeof(int);

	m_iTotal = m_iKor + m_iEng + m_iMat;
	m_fAverage = m_iTotal / 3.0f;
}