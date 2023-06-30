#include "TStd.h"

struct TWidget
{
    int m_iID;
    int a;
    int b;
    int c;
    int d;
    int e;
    TWidget()
    {
        m_iID = 0;
        a = b = c = d = e = 999;
    }
    ~TWidget()
    {
        std::cout << "~TWidget";
    }
    //friend std::ostream& operator << (  std::ostream& os, 
    //                                    const TWidget& s);
};

int main()
{   
    TLinkedlist<TWidget> list;
    list.Init();
    for (int i = 0; i < 3; i++)
    {
        TNode<TWidget>* pNewNode = new TNode<TWidget>;
        pNewNode->m_pData = new TWidget;
        pNewNode->m_pData->a = i;
        list.Push_Back(pNewNode);
    }   
    list.Release();

    printf("아무키나 누르시오!");
    _getche();
}

