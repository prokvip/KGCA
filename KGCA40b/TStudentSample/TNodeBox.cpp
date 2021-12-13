#include "TNodeBox.h"

TStudent* TNodeBox::NewData(int iIndex)
{
	int iType = rand() % 3;
	TStudent* pUser = nullptr;
	if( iType == 0)
		pUser = new TStudentMiddle();
	else if (iType == 1)
		pUser = new TStudentHigh();
	else
		pUser = new TStudentCollege();

	pUser->SetData(iIndex);
	return pUser;
}
std::ostream& operator << (
	std::ostream& os,
	TNodeBox& data)
{
	os << *data.m_pData;
	return os;
}
TNodeBox::TNodeBox()
{
	m_pData = nullptr;
}
TNodeBox::TNodeBox(int iIndex)
{
	m_pData = NewData(iIndex);
}
TNodeBox::~TNodeBox()
{
	delete m_pData;
}
