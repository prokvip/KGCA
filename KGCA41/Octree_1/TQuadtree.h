#pragma once
#include "TNode.h"
class TQuadtreeNode : public TNode
{    
public:
    virtual TNode* CreateNode()
    {
        return new TQuadtreeNode(g_iCounter++);
    }
    virtual void  CreateChildNode()
    {
        for (int i = 0; i < 4; i++)
        {
            TNode* pNewNode = CreateNode();
            pNewNode->SetParent(this);
            m_pChild.push_back(pNewNode);
            std::cout << pNewNode->m_iIndex << " ";
        }
    }
    TNode* CreateNode(TNode* pParent, float x,
        float y,
        float fWidth,
        float fHeight);
public:
    TQuadtreeNode(int id) : TNode(id)
    {
    }
    TQuadtreeNode(TNode* pParent, float x, float y, float fWidth, float fHeight);
    virtual ~TQuadtreeNode()
    {
        for (int i = 0; i < m_pChild.size(); i++)
        {
            delete m_pChild[i];
        }
    }
};

class TQuadtree
{
public:
    TNode*              m_pRootNode = nullptr;
    int                 m_iNumNodeCounter = 0;
    std::set<TNode*>    m_DynamicObjectNodeList;
    std::vector<TNode*> m_NodeArrayList;
public:
    void            BuildQuadTree();
    void            BuildTree(TNode* pParent);
    void            AddNode(TNode* pParent);
    TNode*          CreateNode();
    TNode*          CreateNode(  TNode* pParent, float x,
                        float y,
                        float fWidth,
                        float fHeight);
public:
    std::queue<TNode*> g_Queue;
    void            PreOrder(TNode* pNode);    
    void            LevelOrder(TNode* pNode);
public:
    bool	        Init();		// 초기화 작업
    bool	        PreFrame();	// 실시간 계산
    bool	        Frame();	// 실시간 계산
    bool	        PostFrame();	// 실시간 계산
    bool	        Render();	// 실시간 랜더링, 드로우
    bool	        Release();	// 객체의 소멸 작업
public:
    TNode*          StaticAddObject(TObject* obj);
    TNode*          DynamicAddObject(TObject* obj);
    TNode*          FindNode(TNode* pNode, TObject* obj);
public:
    TQuadtree();    
    virtual ~TQuadtree();
};

