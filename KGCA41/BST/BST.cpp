// BST.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
struct TNode
{
    int    m_iValue = -1;
    TNode* m_pLeftChild = nullptr;
    TNode* m_pRightChild = nullptr;
    TNode(int i) : m_iValue(i)
    {
    }
};
TNode* g_pRootNode = nullptr;

int   g_iData[] = { 50,76,21,4,32,64,15,14,100,83,2,3,70,87,80 };
int   g_iIndex=0;
void  AddNode(TNode* pParentNode, TNode* pNewNode)
{
    if (pParentNode->m_iValue >= pNewNode->m_iValue)
    {
        if (pParentNode->m_pLeftChild != nullptr)
        {
            AddNode(pParentNode->m_pLeftChild, pNewNode);
        }
        else
        {
            pParentNode->m_pLeftChild = pNewNode;            
        }
    }
    else
    {
        if (pParentNode->m_pRightChild != nullptr)
        {
            AddNode(pParentNode->m_pRightChild, pNewNode);
        }
        else
        {
            pParentNode->m_pRightChild = pNewNode;            
        }
    }
}
TNode*  Find(TNode* pParent, int iFind)
{
    if (pParent->m_iValue == iFind)
    {
        return pParent;
    }else
    {
        if (pParent->m_iValue > iFind)
        {
            if (pParent->m_pLeftChild != nullptr)
            {
                Find(pParent->m_pLeftChild, iFind);
            }
            else
            {
                return nullptr;
            }
        } 
        else
        {
            if (pParent->m_pRightChild != nullptr)
            {
                Find(pParent->m_pRightChild, iFind);
            }
            else
            {
                return nullptr;
            }
        }
    }   
}
int main()
{
    g_pRootNode = new TNode(g_iData[g_iIndex++]);
    int iCnt = _countof(g_iData);
    for (int i = 1; i < iCnt; i++)
    {
        AddNode(g_pRootNode, new TNode(g_iData[g_iIndex++]));
    }

    TNode * pFind = Find(g_pRootNode, g_iData[10]);
    pFind = Find(g_pRootNode, 99);
    std::cout << "Hello World!\n";
}
