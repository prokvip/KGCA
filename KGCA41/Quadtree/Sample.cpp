﻿// BST.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <queue>
#include <vector>
struct TNode
{
    int    m_iValue = -1;
    int    m_iDepth = 0;
    TNode* m_pParent = nullptr;
    std::vector<TNode*> m_pChild;
    void   SetParent(TNode* pParent)
    {
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }
    TNode(int i) : m_iValue(i)
    {
        for (int i = 0; i < m_pChild.size(); i++)
        {
            m_pChild[i] = nullptr;
        }
    }
};
int   g_iCounter = 0;
TNode* g_pRootNode = nullptr;
void AddNode(TNode* pNode)
{
    if (pNode->m_iDepth > 2)
    {
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        TNode* pNewNode = new TNode(g_iCounter++);
        pNewNode->SetParent(pNode);
        pNode->m_pChild.push_back(pNewNode);
        std::cout << pNewNode->m_iValue << " ";
    }    
    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        AddNode(pNode->m_pChild[i]);
    }
}
void BuildQuadtree()
{
    g_pRootNode = new TNode(g_iCounter++);
    AddNode(g_pRootNode);
}

void PreOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::cout << pNode->m_iValue << " ";
    for (TNode* data : pNode->m_pChild)
    {
        PreOrder(data);
    }    
}
void PostOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    for (TNode* data : pNode->m_pChild)
    {
        PostOrder(data);
    }
    std::cout << pNode->m_iValue << " ";
}
std::queue<TNode*> g_Queue;
void LevelOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::cout << pNode->m_iValue << " ";
    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            g_Queue.push(pNode->m_pChild[i]);
        }
    }
    if (!g_Queue.empty())
    {
        TNode* pNode = g_Queue.front();
        g_Queue.pop();
        LevelOrder(pNode);
    }
}
int main()
{
    BuildQuadtree();
    std::cout << std::endl;
    // P L R
    PreOrder(g_pRootNode); std::cout << std::endl;   
    // L R P
    PostOrder(g_pRootNode); std::cout << std::endl;
    // Level Order
    LevelOrder(g_pRootNode);
    std::cout << "Hello World!\n";
}