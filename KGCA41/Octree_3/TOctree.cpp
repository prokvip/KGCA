#include "TOctree.h"

bool	TOctree::Init()
{
    return true;
}
bool	TOctree::PreFrame()
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
bool	TOctree::Frame()
{
    return true;
}
bool	TOctree::PostFrame()
{
    return true;
}
bool	TOctree::Render()
{
    return true;
}
bool	TOctree::Release()
{
    return true;
}


void TOctree::PreOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_DynamicObjectList.clear();
    for (TNode* data : pNode->m_pChild)
    {
        PreOrder(data);
    }
}
void TOctree::LevelOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::wcout << L"[" << pNode->m_iDepth << L"]"
        << pNode->m_tBox.v.x << L","
        << pNode->m_tBox.v.y << L","
        << pNode->m_tBox.v.z << L","
        << pNode->m_tBox.m_fWidth << L","
        << pNode->m_tBox.m_fHeight << L","
        << pNode->m_tBox.m_fDepth << L","
        << pNode->m_StaticObjectList3D.size() << L","
        << pNode->m_DynamicObjectList3D.size() << std::endl;
    for (int iobj = 0; iobj < pNode->m_DynamicObjectList3D.size(); iobj++)
    {
        std::wcout << L"      " << L"<" << iobj << L">"
            << pNode->m_DynamicObjectList3D[iobj]->m_csName << L","
            << pNode->m_DynamicObjectList3D[iobj]->m_Position.x << L","
            << pNode->m_DynamicObjectList3D[iobj]->m_Position.y << L","
            << pNode->m_DynamicObjectList3D[iobj]->m_Position.z
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

TOctree::TOctree()
{
}

TOctree::~TOctree()
{
    delete m_pRootNode;
}