#include "TStudentManager.h"
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
TStudent* TStudentManager::NewNode()
{
	TStudent* pNewNode = new TStudent;
	pNewNode->m_iAge = TLinkedList::m_iCounter + 1;
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
void TStudentManager::Print(TStudent* pNode)
{
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%-6.2f\n",
		pNode->m_szName,
		pNode->m_iAge,
		pNode->m_iKor,
		pNode->m_iEng,
		pNode->m_iMat,
		pNode->m_iTotal,
		pNode->m_fAverage);
}
void TStudentManager::Print(int index)
{
	TStudent* pFind = m_List.Find(index);
	if (pFind == nullptr) return;
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%-6.2f\n",
		pFind->m_szName,
		pFind->m_iAge,
		pFind->m_iKor,
		pFind->m_iEng,
		pFind->m_iMat,
		pFind->m_iTotal,
		pFind->m_fAverage);
}
void TStudentManager::AllPrint()
{
	for (int iData = 0; iData < m_List.size(); iData++)
	{
		Print(m_List[iData]);
	}

	for (TStudent* pNode = m_List.begin();
		pNode != m_List.end();
		pNode = m_List.next())
	{
		Print(pNode);
	}

	for (TStudent* pNode = m_List.begin();
		pNode != m_List.end();
		pNode = ++m_List)
	{
		Print(pNode);
	}	
}