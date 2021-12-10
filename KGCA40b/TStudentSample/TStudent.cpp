#include "TStudent.h"

int& TStudent::operator [] (int id)
{
	if (id == 0) return m_iIndex;
	else if (id == 1)	return m_iKor;
	else if (id == 2)	return m_iEng;
	else if (id == 3)	return m_iMat;	
	return m_iTotal;	
}
std::ostream& operator << (
	std::ostream& os,
	TStudent& data)
{
	os << data[0] << " " << data[1] << " "
		<< data[2] << " " << data[3] << " "
		<< data[4] << " "
		<< data.m_fAverage << std::endl;
	return os;
}