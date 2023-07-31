#include "TQuadtree.h"

bool	TQuadtree::Init()
{
    return true;
}
bool	TQuadtree::PreFrame()
{
    // 力1规过(std::set)
    for (auto node : m_DynamicObjectNodeList)
    {
        node->m_DynamicObjectList.clear();
    }
    // 力2规过(犁蓖龋免 滚傈)
    //quadtree.PreOrder(m_pRootNode);

    // 力3规过( 硅凯 滚傈)
    /*for (int i=0; i < g_DynamicNodeList.size() ; i++)
    {
        m_NodeArrayList[i]->m_DynamicObjectList.clear();
    }*/

    m_DynamicObjectNodeList.clear();
    return true;
}
bool	TQuadtree::Frame()
{
    return true;
}
bool	TQuadtree::PostFrame()
{
    return true;
}
bool	TQuadtree::Render()
{
    return true;
}
bool	TQuadtree::Release()
{
    return true;
}


void TQuadtree::PreOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_DynamicObjectList.clear();
    for (TNode* data : pNode->m_pChild)
    {
        PreOrder(data);
    }
}
void TQuadtree::LevelOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::wcout << L"[" << pNode->m_iDepth << L"]"
        << pNode->m_tRT.v.x << L","
        << pNode->m_tRT.v.y << L","
        << pNode->m_tRT.m_fWidth << L","
        << pNode->m_tRT.m_fHeight << L","
        //<< pNode->m_StaticObjectList.size()
        << pNode->m_DynamicObjectList.size() << std::endl;
    for (int iobj = 0; iobj < pNode->m_DynamicObjectList.size(); iobj++)
    {
        std::wcout << L"      " << L"<" << iobj << L">"
            << pNode->m_DynamicObjectList[iobj]->m_csName << L","
            << pNode->m_DynamicObjectList[iobj]->m_Position.x << L","
            << pNode->m_DynamicObjectList[iobj]->m_Position.y
            << std::endl;
    }
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

TQuadtree::TQuadtree() 
{
}

TQuadtree::~TQuadtree()
{
    delete m_pRootNode;
}