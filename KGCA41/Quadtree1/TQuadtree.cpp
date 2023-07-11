#include "TQuadtree.h"
TNode* TQuadtree::FindNode(TNode* pNode, TObject* obj)
{
    if (pNode == nullptr) return nullptr;
    do
    {
        for (int i = 0; i < pNode->m_pChild.size(); i++)
        {
            if (pNode->m_pChild[i] != nullptr)
            {
                if (TCollision::RectToPoint(pNode->m_pChild[i]->m_rt,
                    obj->m_Position))
                {
                    g_Queue.push(pNode->m_pChild[i]);
                    break;
                }
            }
        }
        if (g_Queue.empty()) break;
        pNode = g_Queue.front();
        g_Queue.pop();        
    }while (pNode);
    return pNode;
}
TNode* TQuadtree::AddObject(TObject* obj)
{
    TNode* pFindNode = FindNode(g_pRootNode, obj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectList.push_back(obj);
        return pFindNode;
    }    
    return nullptr;
}
TNode* TQuadtree::GetRootNode()
{
    return g_pRootNode;
}
TNode* TQuadtree::CreateNode(TNode* pParent, float x,
    float y,
    float fWidth,
    float fHeight)
{
    return new TNode(pParent,x,y,fWidth, fHeight);
}
void TQuadtree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth > 1)
    {
        return;
    }
    // 0, 0,  800, 600
    // v0-------vTC-------v1
    // |                  |
    // |                  |
    // vLC      vC       vRC
    // |                  |
    // |                  |
    // v3       vBC       v2
    
    TPoint vTC = { pNode->m_rt.m_Center.x, pNode->m_rt.m_Point[0].y };
    TPoint vRC = { pNode->m_rt.m_Point[1].x, pNode->m_rt.m_Center.y };
    TPoint vBC = { pNode->m_rt.m_Center.x, pNode->m_rt.m_Point[3].y };
    TPoint vLC = { pNode->m_rt.m_Point[0].x, pNode->m_rt.m_Center.y };
  
    TNode* pNewNode = CreateNode(pNode, 
        pNode->m_rt.m_Point[0].x,
        pNode->m_rt.m_Point[0].y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);

    pNewNode = CreateNode(pNode,
        vTC.x,
        vTC.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);

    pNewNode = CreateNode(pNode,
        pNode->m_rt.m_Center.x,
        pNode->m_rt.m_Center.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);    

    pNewNode = CreateNode(pNode,
        vLC.x,
        vLC.y,
        pNode->m_rt.m_Half.x,
        pNode->m_rt.m_Half.y);
    pNode->m_pChild.push_back(pNewNode);
   

    std::cout << "[" << pNode->m_iDepth << "]" 
        << pNode->m_rt.m_fx << ","
        << pNode->m_rt.m_fy << ","
        << pNode->m_rt.m_fWidth << ","
        << pNode->m_rt.m_fHeight  << std::endl;

    for (int i = 0; i < pNode->m_pChild.size(); i++)
    {
        Buildtree(pNode->m_pChild[i]);
    }
}
void TQuadtree::BuildQuadtree()
{
    g_pRootNode = new TNode(nullptr, 0, 0, 800, 600);    
    Buildtree(g_pRootNode);
}

void TQuadtree::PreOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    //std::cout << pNode->m_iValue << " ";
    for (TNode* data : pNode->m_pChild)
    {
        PreOrder(data);
    }
}
void TQuadtree::PostOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    for (TNode* data : pNode->m_pChild)
    {
        PostOrder(data);
    }
    //std::cout << pNode->m_iValue << " ";
}

void TQuadtree::LevelOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::wcout <<   L"[" << pNode->m_iDepth << L"]"
        << pNode->m_rt.m_fx << L","
        << pNode->m_rt.m_fy << L","
        << pNode->m_rt.m_fWidth << L","
        << pNode->m_rt.m_fHeight << L","
        << pNode->m_ObjectList.size() << std::endl;
    for (int iobj = 0; iobj < pNode->m_ObjectList.size(); iobj++)
    {
        std::wcout << L"      " << L"<" << iobj << L">"
            << pNode->m_ObjectList[iobj]->m_csName << L","
            << pNode->m_ObjectList[iobj]->m_Position.x << L","
            << pNode->m_ObjectList[iobj]->m_Position.y 
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