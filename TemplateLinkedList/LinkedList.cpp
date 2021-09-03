#include "TLinkedList.h"
#include "T1.h"
//#define USE_STACK
struct TData
{
    int iIndex;
};
int main()
{
    Node<int> a;
    a.m_Value = 99;
    int ret = a.Get();
    Node<float> b;
    a.m_Value = 99.9f;
    ret = a.Get();

    TLinkedList<TData> t;    
    TData data;
    for (int i = 0; i < 26; i++)
    {
        TNode<TData>* a = new TNode<TData>;
        data.iIndex = i;
        a->m_Value = data;
#ifdef USE_STACK
        t.AddForwardLink(a);
#else
        t.AddBackwardLink(a);
#endif
    }
    //t.Show();
    
    for (TNode<TData>* pNode = t.m_pHead->m_pNext;
        pNode != t.m_pTail;
        pNode = pNode->m_pNext)
    {
        TData* data = &pNode->m_Value;
        std::cout << data->iIndex << "\n";
    }
    

    for (int i = 0; i < 26; i++)
    {
        std::cout << t.Pop().iIndex << " ";
    }

    std::cout << "Hello World!\n";
}
