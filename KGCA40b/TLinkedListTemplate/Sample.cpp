#include "TLinkedList.cpp"

struct TCharacter
{
	int iValue;
};

void main()
{
	TLinkedList<TCharacter> list;
	for (int i = 0; i < 5; i++)
	{
		//error C4700 : �ʱ�ȭ���� ���� 'data' ���� ������ ����߽��ϴ�.
		TCharacter data = { i };
		//list.PushBack(new TNode<int>(i));
		list.PushFront(new TNode<TCharacter>(data));
	}
	TNode<TCharacter>* pFind = list.find(3);
	list.erase(pFind);

	for (TNode<TCharacter>* pNode = list.begin();
		pNode != list.end();
		pNode = list++) 		
	{
		std::cout << pNode->m_pData.iValue << " ";
	}
	std::cout << "end" << std::endl;

	for (TNode<TCharacter>* pNode = list.rbegin();
		pNode != list.rend();
		pNode = ++list)
	{
		std::cout << pNode->m_pData.iValue << " ";
	}

	std::cout << "end" << std::endl;

	list.clear();
}

