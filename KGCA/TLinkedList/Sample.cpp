#include "TLinkedList.h"

void main()
{
	TLinkedList list;
	for (int i = 0; i < 5; i++)
	{
		//list.PushBack(new TNode(i));
		list.PushFront(new TNode(i));
	}
	TNode* pFind = list.find(3);
	list.erase(pFind);

	for (TNode* pNode = list.begin();
		pNode != list.end();
		pNode = list++) 		
	{
		std::cout << pNode->m_iValue << " ";
	}
	std::cout << "end" << std::endl;

	for (TNode* pNode = list.rbegin();
		pNode != list.rend();
		pNode = ++list)
	{
		std::cout << pNode->m_iValue << " ";
	}

	std::cout << "end" << std::endl;

	list.clear();
}

