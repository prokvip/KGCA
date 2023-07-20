#pragma once
#include "TNode.h"
class TOctree : public TNode
{
public:
    virtual TNode* CreateNode()
    {
        return new TOctree(g_iCounter++);
    }
    virtual void  CreateChildNode()
    {
        for (int i = 0; i < 8; i++)
        {
            TNode* pNewNode = CreateNode();
            pNewNode->SetParent(this);
            m_pChild.push_back(pNewNode);
            std::cout << pNewNode->m_iIndex << " ";
        }
    }
public:
    TOctree(int id) : TNode(id)
    {
        /*for (int i = 0; i < m_pChild.size(); i++)
        {
            m_pChild[i] = nullptr;
        }*/
    }
    virtual ~TOctree()
    {
        for (int i = 0; i < m_pChild.size(); i++)
        {
            delete m_pChild[i];
        }
    }
};

