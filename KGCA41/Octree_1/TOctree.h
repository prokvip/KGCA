#pragma once
#include "TNode.h"

class TOctreeNode : public TNode
{
public:
    virtual TNode* CreateNode()
    {
        return new TOctreeNode(g_iCounter++);
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
    TOctreeNode(int id) : TNode(id)
    {
        /*for (int i = 0; i < m_pChild.size(); i++)
        {
            m_pChild[i] = nullptr;
        }*/
    }
    virtual ~TOctreeNode()
    {
        for (int i = 0; i < m_pChild.size(); i++)
        {
            delete m_pChild[i];
        }
    }
};


class TOctree 
{
public:
    virtual TNode* CreateNode()
    {
        return new TOctreeNode(g_iCounter++);
    }   
public:
    TOctree() 
    {       
    }
    virtual ~TOctree()
    {
       
    }
};

