#include "TLinkedList.h"
//#define USE_STACK
int main()
{
    TLinkedList t;
    for (int i = 0; i < 30; i++)
    {
        TNode* a = new TNode;
        a->m_iValue = i;
#ifdef USE_STACK
        t.AddForwardLink(a);
#else
        t.AddBackwardLink(a);
#endif
    }
    t.Show();
    for (int i = 0; i < 30; i++)
    {
        std::cout << t.Pop() << " ";
    }

    std::cout << "Hello World!\n";
}
