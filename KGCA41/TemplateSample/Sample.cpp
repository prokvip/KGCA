#include "THashTable.h"
struct TItem 
{
	int iValue;	
	char name[4];
	TItem(int i) : iValue(i) {
		name[0] = 65+rand() % 26;
		name[1] = 65 + rand() % 26;
		name[2] = 97+rand() % 26;
		name[3] = 0;
	}
};

template<class T>
void Print(TLinkedList<T>& list)
{
	for (TNode<T>* node = list.m_pHead->m_pNext;
		node != list.m_pTail;
		node = node->m_pNext)
	{
		int iData = node->m_pData;
		std::cout << iData << " ";
	}
	std::cout << std::endl;
}
void main()
{
	srand(time(NULL));
	TTable<int, TItem*> list;
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		TItem* pItem = new TItem(10+iCnt);
		list.insert(iCnt, pItem);
	}	
	using DATA = TBucket<int, TItem*>*;
	using TNODE = TNode<DATA>*;
	for (TNODE pBucket = list.m_Table.begin();
		 pBucket != list.m_Table.end();
		 pBucket = list.m_Table.next())
	{
		DATA pData = pBucket->m_pData;
		std::cout << pData->m_Key << ":" << pData->m_pData->iValue << std::endl;
	}
	std::cout << std::endl;	

	TTable<char*, TItem*> list2;
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		TItem* pItem = new TItem(10 + iCnt);
		list2.insert(pItem->name, pItem);
	}
	using DATA2 = TBucket<char*, TItem*>*;
	using TNODE2 = TNode<DATA2>*;
	for (TNODE2 pBucket = list2.m_Table.begin();
		pBucket != list2.m_Table.end();
		pBucket = list2.m_Table.next())
	{
		DATA2 pData = pBucket->m_pData;
		std::cout << pData->m_Key << ":" << pData->m_pData->iValue << std::endl;
	}
	std::cout << std::endl;

	list.clear();
	list2.clear();
}