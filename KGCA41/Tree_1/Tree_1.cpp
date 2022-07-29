// Tree_1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "THashTable.h"
//enum TTreeDir { T_LEFT = 0, T_RIGHT, };
//class TNode
//{
//public:
//	int		m_iIndex;
//	int		m_iDepth;
//	TNode* m_pChild[2];
//	TNode* m_pParent;
//	TNode() : m_iIndex(-1), m_iDepth(0)
//	{
//		for (int iNode = 0; iNode < 2; iNode++)
//		{
//			m_pChild[iNode] = nullptr;
//		}
//		m_pParent = nullptr;
//	}
//	virtual ~TNode()
//	{
//		std::cout << this->m_iIndex << " ";
//	}
//};
int g_iData[] = { 5,23,67,3,7,3,6,3,1,8 };
//int g_iCounter = 0;
//TNode* g_pRoot = nullptr;
//
//void CreateBinarySearchTree(TNode* pNode, int iValue)
//{
//	// 루트 >= iValue ==> 왼쪽
//	if( pNode->m_iIndex >= iValue)
//	{
//		if (pNode->m_pChild[0] == nullptr)
//		{
//			pNode->m_pChild[0] = new TNode();
//			pNode->m_pChild[0]->m_iIndex = iValue;
//			pNode->m_pChild[0]->m_iDepth = pNode->m_iDepth + 1;
//		}
//		else
//		{
//			CreateBinarySearchTree(pNode->m_pChild[0], iValue);
//		}
//	}
//	// 루트 <  iValue ==> 오른쪽
//	else
//	{ 
//		if (pNode->m_pChild[1] == nullptr)
//		{
//			pNode->m_pChild[1] = new TNode();
//			pNode->m_pChild[1]->m_iIndex = iValue;
//			pNode->m_pChild[1]->m_iDepth = pNode->m_iDepth + 1;
//		}
//		else
//		{
//			CreateBinarySearchTree(pNode->m_pChild[1], iValue);
//		}
//	}
//}
//TNode* BinarySearchTree(TNode* pNode, int iValue)
//{
//	if (pNode == nullptr) return nullptr;
//	if (pNode->m_iIndex == iValue) return pNode;
//	if (pNode->m_iIndex >= iValue)
//	{
//		BinarySearchTree(pNode->m_pChild[0], iValue);		
//	}
//	else
//	{	
//		BinarySearchTree(pNode->m_pChild[1], iValue);		
//	}
//}

struct TItem
{
	int iValue;
	char name[4];
	TItem(int i) : iValue(i) {
		name[0] = 65 + rand() % 26;
		name[1] = 65 + rand() % 26;
		name[2] = 97 + rand() % 26;
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
		TItem* pItem = new TItem(rand());
		list.insert(pItem->iValue, pItem);
	}
	TItem* pFindItem = list.bst(8);

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
	list.clear();
	/*TTable<char*, TItem*> list2;
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
	list2.clear();*/
}

//int main()
//{
//	g_pRoot = new TNode();
//	g_pRoot->m_iIndex = g_iData[g_iCounter++];	
//	for (int i = 1; i < _countof(g_iData); i++)
//	{
//		CreateBinarySearchTree(g_pRoot, g_iData[i]);
//	}
//
//	for (int i = 0; i < _countof(g_iData); i++)
//	{
//		TNode* pFind = BinarySearchTree(g_pRoot, g_iData[i]);
//		if (pFind != nullptr)
//		{
//			std::cout << " "<< pFind->m_iIndex;
//		}
//		else
//		{
//			int k = 0;
//		}
//	}
//	std::cout << "Hello World!\n";
//}
