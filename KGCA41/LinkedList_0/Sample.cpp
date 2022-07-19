#include "TLinkedList.h"
struct TItem 
{
	int iValue;	
	TItem(int i) : iValue(i) {}
};
void main()
{	
	TLinkedList<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	for (TNode<int>* node = list.m_pHead->m_pNext;
		node != list.m_pTail;
		node = node->m_pNext)
	{		
		int iData = node->m_pData;
		std::cout << iData << std::endl;
	}

	TLinkedList<char> list1;
	list1.push_back(65);
	list1.push_back(66);
	list1.push_back(67);

	for (TNode<char>* node = list1.m_pHead->m_pNext;
		node != list1.m_pTail;
		node = node->m_pNext)
	{
		char iData = node->m_pData;
		std::cout << iData << std::endl;
	}

	TLinkedList<TItem*> list2;
	list2.push_back(new TItem(4));
	list2.push_back(new TItem(5));
	list2.push_back(new TItem(6));

	for (TNode<TItem*>* node = list2.m_pHead->m_pNext;
		node != list2.m_pTail;
		node = node->m_pNext)
	{
		TItem* pData = node->m_pData;
		std::cout << pData->iValue << std::endl;
	}
	std::cout << "aa";
	//TStudentManager mgr;
	
	//srand(time(NULL));
	//int iWork = 1;
	//while (iWork)
	//{
	//	int iSelect = 0;
	//	printf("\n->[0:Create],[1:Print],[2:Find],[3:Del],[4:insert],[Quit:999]  : ");
	//	scanf("%d", &iSelect);
	//	if (iSelect == 999) break;

	//	Clear();

	//	switch (iSelect)
	//	{
	//		case 0:
	//		{				
	//			mgr.InitData(3);
	//		}break;
	//		case 1:
	//		{
	//			//std::cout << mgr;//	
	//			mgr.AllPrint();
	//		}break;
	//		case 2:
	//		{
	//			TItem* pFind = mgr.m_List.Find(3);
	//			if (pFind != nullptr)
	//			{
	//				mgr.Print(pFind);
	//			}
	//		}break;
	//		case 3 :
	//		{
	//			TItem* pFind = mgr.m_List.Find(3);
	//			if (pFind != nullptr)
	//			{
	//				mgr.Print(pFind);
	//				mgr.m_List.erase(pFind);
	//				system("cls");
	//				mgr.AllPrint();
	//			}
	//		}break;
	//		case 4:
	//		{
	//			mgr.m_List.push_back(mgr.NewNode());
	//		}break;
	//		case 5:
	//		{
	//			TItem* pFind = mgr.m_List.Find(2);
	//			if (pFind != nullptr)
	//			{
	//				mgr.m_List.push_front(mgr.NewNode(), pFind);
	//				mgr.AllPrint();
	//			}
	//		}break;
	//	}
	//}	
	//mgr.m_List.clear();
}