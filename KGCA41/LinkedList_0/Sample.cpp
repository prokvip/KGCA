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

bool Ascending(TNode<int>* a, TNode<int>* b) //오름차순
{
	if (a->m_pData < b->m_pData)
	{
		return true;
	}
	return false;
}
bool Descending(TNode<int>* a, TNode<int>* b) //내림차순
{
	if (a->m_pData > b->m_pData)
	{
		return true;
	}
	return false;
}
bool AscendingItem(TNode<TItem*>* a, TNode<TItem*>* b) //오름차순
{
	if (a->m_pData->iValue < b->m_pData->iValue)
	{
		return true;
	}
	return false;
}
bool DescendingItem(TNode<TItem*>* a, TNode<TItem*>* b) //내림차순
{
	if (a->m_pData->iValue > b->m_pData->iValue)
	{
		return true;
	}
	return false;
}

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
	TLinkedList<int> list;
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		int iValue = rand() % 100;
		list.push_back(iValue);
		std::cout << iValue << " ";
	}
	std::cout << std::endl;
	list.sort();
	Print(list);

	list.CallFunction = &TLinkedList<int>::Ascending;
	
	list.sort();
	Print(list);
	
	list.shuffle();
	Print(list);
	
	list.sort_if(Descending);// Ascending);
	Print(list);
	list.shuffle();

	Print(list);
	list.sort();
	Print(list);

	list.clear();
	list.sort();
	std::cout << std::endl;

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
	//list2.sort();
	list2.sort_if(AscendingItem);
	std::cout << std::endl;

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