#include "TStudentManager.h"
std::ostream& operator << (std::ostream& os, TStudentManager& mgr)
{
	//os << mgr.m_List;
	return os;
}
std::istream& operator >> (std::iostream& is, TStudentManager& mgr)
{
	//is >> mgr.m_List;
	return is;
}
void TStudentManager::InitData(int iCounter)
{
	//for (int iNode = 0; iNode < iCounter; iNode++)
	//{
	//	m_List.push(NewNode());
	//}
	for (int iNode = iCounter; iNode < iCounter * 2; iNode++)
	{
		m_List.push_front(NewNode());
	}
}
TItem* TStudentManager::NewNode()
{
	TItem* pNewNode = new TItem;
	pNewNode->m_iAge = TLinkedList<TItem>::m_iCounter + 1;
	pNewNode->m_szName[0] = 65 + rand() % 26;
	pNewNode->m_szName[1] = 65 + rand() % 26;
	pNewNode->m_szName[2] = 65 + rand() % 26;
	pNewNode->m_szName[3] = 0;
	pNewNode->m_iKor = 0 + rand() % 101;
	pNewNode->m_iEng = 0 + rand() % 101;
	pNewNode->m_iMat = 0 + rand() % 101;
	pNewNode->m_iTotal = pNewNode->m_iKor +
		pNewNode->m_iEng +
		pNewNode->m_iMat;
	pNewNode->m_fAverage = pNewNode->m_iTotal / 3;
	return pNewNode;
}
void TStudentManager::Print(TItem* pNode)
{
	pNode->Print();
}
void TStudentManager::Print(int index)
{
	TItem* pFind = m_List.Find(index);
	if (pFind == nullptr) return;
	pFind->Print();
}
void TStudentManager::AllPrint()
{
	//for (int iData = 0; iData < m_List.size(); iData++)
	//{
	//	Print(m_List[iData]);
	//}
	for (TItem* pNode = m_List.begin();
		pNode != m_List.end();
		pNode = m_List.next())
	{
		std::cout << *pNode;
	}

	//for (TItem* pNode = m_List.begin();
	//	pNode != m_List.end();
	//	pNode = ++m_List)
	//{
	//	Print(pNode);
	//}	
}