#include "TStd.h"

struct TWidget
{
    int m_iID;
    int a;
    int b;
    int c;
    int d;
    int e;
};

int main()
{   
    TLinkedlist<TWidget> list;
    list.Init();
    for (int i = 0; i < 3; i++)
    {
        TNode<TWidget>* pNewNode = new TNode<TWidget>;
        pNewNode->m_pData.a = i;
        list.Push_Back(pNewNode);
    }
    TLinkedlist<int> listInt;
    listInt.Init();
    for (int i = 0; i < 3; i++)
    {
        TNode<int>* pNewNode = new TNode<int>;
        pNewNode->m_pData = 1000 - i;
        listInt.Push_Back(pNewNode);
    }
    for (int i = 0; i < 3; i++)
    {
        TNode<int>* pNewNode = new TNode<int>;
        pNewNode->m_pData = 1000 - i;
        listInt.Push_Front(pNewNode);
    }
    list.Release();
    listInt.Release();

    printf("아무키나 누르시오!");
    _getche();
}

