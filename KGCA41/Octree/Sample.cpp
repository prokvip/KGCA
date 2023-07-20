// BST.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <queue>
#include <vector>

int   g_iCounter = 0;

class TNode
{
public:
    int    m_iIndex = -1;
    int    m_iDepth = 0;
    TNode* m_pParent = nullptr;
    std::vector<TNode*> m_pChild;
    virtual TNode* CreateNode()
    {
        return new TNode(g_iCounter++);
    };
    virtual void   CreateChildNode()
    {
    };
    virtual void   SetParent(TNode* pParent)
    {
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }
    TNode(int id) : m_iIndex(id)
    {        
    }
    virtual ~TNode() {}
};
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
public:
    TQuadtreeNode(int id) : TNode(id)
    {        
    }
    virtual ~TQuadtreeNode() 
    {
        for (int i = 0; i < m_pChild.size(); i++)
        {
            delete m_pChild[i];
        }
    }
};
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

TNode* g_pRootNode = nullptr;
void AddNode(TNode* pParent)
{
    if (pParent->m_iDepth > 1)
    {
        return;
    }
    pParent->CreateChildNode();

    for (int i = 0; i < pParent->m_pChild.size(); i++)
    {
        AddNode(pParent->m_pChild[i]);
    }
}
void BuildQuadtree()
{
    g_pRootNode = new TQuadtreeNode(g_iCounter++);
    AddNode(g_pRootNode);
}
void BuildOctree()
{
    g_pRootNode = new TOctreeNode(g_iCounter++);
    AddNode(g_pRootNode);
}

void PreOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::cout << pNode->m_iIndex << " ";
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
    std::cout << pNode->m_iIndex << " ";
}
std::queue<TNode*> g_Queue;
void LevelOrder(TNode* pNode)
{
    if (pNode == nullptr) return;
    std::cout << pNode->m_iIndex << " ";
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
    //BuildQuadtree();

    BuildOctree();

    std::cout << std::endl;
    // P L R
    PreOrder(g_pRootNode); std::cout << std::endl;   
    //// L R P
    //PostOrder(g_pRootNode); std::cout << std::endl;
    //// Level Order
    //LevelOrder(g_pRootNode);
    // 
    delete g_pRootNode;
    //std::cout << "Hello World!\n";
}
