#include "TLinkedList.h"
struct TItem 
{
	int iValue;	
	TItem(int i) : iValue(i) {}
public:
	friend std::ostream& operator << (std::ostream& os, TItem& list);
	friend std::istream& operator >> (std::istream& is, TItem& list);
};
std::ostream& operator << (std::ostream& os, TItem& data)
{
	os << data.iValue;
	return os;
}
std::istream& operator >> (std::iostream& is, TItem& data)
{
	return is;
}
void main()
{	
	TLinkedList<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_front(3);
	list.push_front(4);

	for (TNode<int>* node = list.m_pHead->m_pNext;
		node != list.m_pTail;
		node = node->m_pNext)
	{		
		int iData = node->m_pData;
		std::cout << iData << std::endl;
	}
	list.clear();

	TLinkedList<TItem*> list2;
	list2.push_back(new TItem(4));
	list2.push_back(new TItem(5));
	TItem* pNewData = new TItem(6);
	list2.push_back(pNewData);
	list2.push_front(new TItem(7));
	list2.push_front(new TItem(8));
		
	for (int iNode=0; iNode < list2.size(); iNode++)
	{
		TItem* pData = list2[iNode]->m_pData;
		std::cout << *pData;
	}

	for (TNode<TItem*>* node = list2.begin();
		node != list2.end();
		node = ++list2)
	{
		TItem* pData = node->m_pData;
		std::cout << pData->iValue << std::endl;
	}

	TItem* pFindData = list2.Find(pNewData);
	std::cout << pFindData->iValue << std::endl;

	for (TNode<TItem*>* node = list2.m_pHead->m_pNext;
		node != list2.m_pTail;
		)
	{
		TItem* pData = node->m_pData;
		delete pData;
		node = list2.erase(node);
	}
	list2.clear();
}