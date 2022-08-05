#include <iostream>
class TNode
{
public:
    float  m_fX;
    float  m_fY;
    float   m_fWidth;
    float   m_fHeight;
    int    m_iDepth;
    TNode* m_pChild[4];
    TNode* m_pParent;
public:
    TNode(TNode* pParent, float x, float y, float w, float h)
    {
        m_fX = x;
        m_fY = y;
        m_fWidth = w;
        m_fHeight = h;
        m_iDepth = 0;
        m_pParent = nullptr;
        if (pParent != nullptr)
        {
            m_pParent = pParent;
            m_iDepth = pParent->m_iDepth + 1;
        }
    }
    ~TNode()
    {
        delete m_pChild[0];
        delete m_pChild[1];
        delete m_pChild[2];
        delete m_pChild[3];
    }
};
class TQuadtree
{
public:
    TNode* m_pRootNode;
public:
    void    Create(float fWidth,float fHeight);
    TNode*  CreateNode(TNode* pParent, 
             float fPosX, float fPosY,
             float fWidth, float fHeight);
    void    Buildtree(TNode* pNode);
    ~TQuadtree()
    {
        delete m_pRootNode;
    }
};
void TQuadtree::Create(
    float fWidth, 
    float fHeight)
{
    m_pRootNode = CreateNode(nullptr,
          0,0,fWidth, fHeight);
    Buildtree(m_pRootNode);
}
void TQuadtree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth >= 2) return;
    if (pNode == nullptr) return;

    float x, y, w,h;
    x = pNode->m_fX;
    y = pNode->m_fY;
    w = pNode->m_fWidth / 2.0f;
    h = pNode->m_fHeight / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode,x,y,w,h);
    x = pNode->m_fX + w;
    y = pNode->m_fY;
    pNode->m_pChild[1] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_fX;
    y = pNode->m_fY + h;
    pNode->m_pChild[2] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_fX + w;
    y = pNode->m_fY + h;
    pNode->m_pChild[3] = CreateNode(pNode, x, y, w, h);

    for (int iChild = 0; iChild < 4; iChild++)
    {
        Buildtree(pNode->m_pChild[iChild]);
    }
}
TNode* TQuadtree::CreateNode(
    TNode* pParent,
    float x, float y,
    float w, float h)
{
    TNode* pNode = new TNode(pParent,x, y, w, h);    
    return pNode;
}
int main()
{
    TQuadtree quadtree;
    quadtree.Create(100.0f, 100.0f);

    //화면좌표계(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
    //0,0 -> x       50,0         100(x), 0(y)        
    //|    0                  1
    //y 0,50         50,50        100,50   
    //     2                  3 
    //0,100          50,100       100,100 
    std::cout << "Hello World!\n";
}
