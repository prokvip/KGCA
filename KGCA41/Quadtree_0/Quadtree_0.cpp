#include <iostream>
#include <vector>
#include <queue>

class TObject
{
public:
    float  m_fX;
    float  m_fY;
    float  m_fWidth;
    float  m_fHeight;
    void   SetPosition(float x, float y, float w, float h)
    {
        m_fX = x;
        m_fY = y;
        m_fWidth = w;
        m_fHeight = h;
    }
    TObject()
    {
        m_fX = 20 + (rand() % 80);
        m_fY = 20 + (rand() % 80);
        m_fWidth = 10.0f + (rand() % 10);
        m_fHeight = 10.0f + (rand() % 10);
    }
};
class TNode
{
public:
    float  m_fX;
    float  m_fY;
    float   m_fWidth;
    float   m_fHeight;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectList;
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

std::queue<TNode*> g_Queue;
class TQuadtree
{
public:
    TNode* m_pRootNode;
public:
    std::vector<TObject*> Collision(TObject* pObj);
    void    Create(float fWidth,float fHeight);
    TNode*  CreateNode(TNode* pParent, 
             float fPosX, float fPosY,
             float fWidth, float fHeight);
    void    Buildtree(TNode* pNode);
    void    AddObject(TObject* pObj);
    TNode*  FindNode(TNode* pNode, TObject* pObj);
    bool    RectToInRect(TNode* pNode, TObject* pObj);
    int     RectToRect(TNode* pNode, TObject* pObj);
    int     RectToRect(TObject* pDestNode, TObject* pSrcObj);
    void    GetCollisitionObject(   TNode* pNode, 
                                    TObject* obj,
                                    std::vector<TObject*>& list);
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
// 1번 : 완전히 포함하는 노드에 추가하자.
// 2번 : 걸쳐만 있어도 노드에 추가하자.
void    TQuadtree::AddObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if( pFindNode != nullptr)
    {
        pFindNode->m_ObjectList.push_back(pObj);
    }    
}
TNode* TQuadtree::FindNode(TNode* pNode, TObject* pObj)
{
    do {
        for (int iNode = 0; iNode < 4; iNode++)
        {
            if (pNode->m_pChild[iNode] != nullptr)
            {
                bool bIn = RectToInRect(pNode->m_pChild[iNode], pObj);
                if (bIn > 0)
                {
                    g_Queue.push(pNode->m_pChild[iNode]);
                    break;
                }
            }
        }
        if (g_Queue.empty()) break;
        pNode = g_Queue.front();
        g_Queue.pop();
    } while (pNode);
    return pNode;
}
bool    TQuadtree::RectToInRect(TNode* pNode, TObject* pObj)
{
    //  |             |
    if (pNode->m_fX <= pObj->m_fX)
    {
        if ((pNode->m_fX+pNode->m_fWidth) >= pObj->m_fX + pObj->m_fWidth)
        {
            if (pNode->m_fY <= pObj->m_fY)
            {
                if ((pNode->m_fY + pNode->m_fHeight) >= pObj->m_fY + pObj->m_fHeight)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
int    TQuadtree::RectToRect(TNode* pNode,   TObject* pObj)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // x1(10)----30------x2(40)
    float x1 = pNode->m_fX;
    float y1 = pNode->m_fY;
    float x2 = pNode->m_fX + pNode->m_fWidth;
    float y2 = pNode->m_fY + pNode->m_fHeight;

    float z1 = pObj->m_fX;
    float w1 = pObj->m_fY;
    float z2 = pObj->m_fX + pObj->m_fWidth;
    float w2 = pObj->m_fY + pObj->m_fHeight;

    // 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = x1 < z1 ? x1 : z1;
    fMinY = y1 < w1 ? y1 : w1;
    fMaxX = x2 < z2 ? x2 : z2;
    fMaxY = y2 < w2 ? y2 : w2;

    //  가로 판정
    if ((pNode->m_fWidth + pObj->m_fWidth) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((pNode->m_fHeight + pObj->m_fHeight) >= (fMaxY - fMinY))
        {
            // 교차한다. 교집합
            return 1;
        }
    }    
    return 0;
}
int    TQuadtree::RectToRect(TObject* pDestObj, TObject* pObj)
{
    // 0 : 완전제외(0)
        // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
        // x1(10)----30------x2(40)
    float x1 = pDestObj->m_fX;
    float y1 = pDestObj->m_fY;
    float x2 = pDestObj->m_fX + pDestObj->m_fWidth;
    float y2 = pDestObj->m_fY + pDestObj->m_fHeight;

    float z1 = pObj->m_fX;
    float w1 = pObj->m_fY;
    float z2 = pObj->m_fX + pObj->m_fWidth;
    float w2 = pObj->m_fY + pObj->m_fHeight;

    // 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = x1 < z1 ? x1 : z1;
    fMinY = y1 < w1 ? y1 : w1;
    fMaxX = x2 > z2 ? x2 : z2;
    fMaxY = y2 > w2 ? y2 : w2;

    //  가로 판정
    if ((pDestObj->m_fWidth + pObj->m_fWidth) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((pDestObj->m_fHeight + pObj->m_fHeight) >= (fMaxY - fMinY))
        {
            // 교차한다. 교집합
            return 1;
        }
    }
    return 0;
}
std::vector<TObject*> TQuadtree::Collision(TObject* pObj)
{
    std::vector<TObject*> list;
    GetCollisitionObject(   this->m_pRootNode, pObj, 
                            list);
    return list;
};
void  TQuadtree::GetCollisitionObject(  TNode* pNode, 
                                        TObject* pSrcObject,
                                        std::vector<TObject*>& list)
{
    if (pNode == nullptr) return;
    for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
    {
        if (RectToRect(pNode->m_ObjectList[iObj], pSrcObject))
        {
            list.push_back(pNode->m_ObjectList[iObj]);
        }
    }
    if (pNode->m_pChild[0] == nullptr)
    {
        for (int iChild = 0; iChild < 4; iChild++)
        {
            if (RectToRect(pNode->m_pChild[iChild], pSrcObject))
            {
                GetCollisitionObject(pNode->m_pChild[iChild], pSrcObject, list);
            }
        }
    }
}
int main()
{
    //std::vector<int> list1 = { 4,5,64, };
    //std::vector<int> list2;
    //list2 = list1;

    TObject player;
    player.SetPosition(10, 10, 30, 30);
    TQuadtree quadtree;
    quadtree.Create(100.0f, 100.0f);
    for (int iObj = 0; iObj < 100; iObj++)
    {
        TObject* pObj = new TObject;
        quadtree.AddObject(pObj);
    }
    //TNode* pNodePlayer = quadtree.FindNode(quadtree.m_pRootNode, &player);
    std::vector<TObject*> list = quadtree.Collision(&player);
    if (!list.empty())
    {
        for (int iObj = 0; iObj < list.size(); iObj++)
        {
            std::cout << list[iObj]->m_fX <<
                         list[iObj]->m_fY << std::endl;
        }
    }
    //while (1)
    //{
    //    //quadtree.MoveObject();
    //}
    //화면좌표계(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
    //0,0 -> x       50,0         100(x), 0(y)        
    //|    0                  1
    //y 0,50         50,50        100,50   
    //     2                  3 
    //0,100          50,100       100,100 
    std::cout << "Hello World!\n";
}
