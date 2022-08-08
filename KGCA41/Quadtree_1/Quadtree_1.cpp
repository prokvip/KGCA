#include <windows.h>
#include "TCollision.h"


class TObject
{
public:
    TRect m_rt;
    void   SetPosition(float x, float y, float w, float h)
    {
        m_rt.Set(x,y,w,h);
    }
    TObject()
    {
        m_rt.Set(   20 + (rand() % 80), 
                    20 + (rand() % 80),
                    5.0f + (rand() % 5),
                    5.0f + (rand() % 5));
    }
};
class TNode
{
public:
    TRect m_rt;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectList;
    TNode* m_pChild[4];
    TNode* m_pParent;
public:
    TNode(TNode* pParent, float x, float y, float w, float h)
    {
        m_rt.Set(x, y, w, h);
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
    void    Create(float fWidth, float fHeight);
    TNode* CreateNode(TNode* pParent,
        float fPosX, float fPosY,
        float fWidth, float fHeight);
    void    Buildtree(TNode* pNode);
    void    AddObject(TObject* pObj);
    TNode* FindNode(TNode* pNode, TObject* pObj);
    void    GetCollisitionObject(TNode* pNode,
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
        0, 0, fWidth, fHeight);
    Buildtree(m_pRootNode);
}
void TQuadtree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth >= 2) return;
    if (pNode == nullptr) return;

    float x, y, w, h;
    x = pNode->m_rt.x;
    y = pNode->m_rt.y;
    w = pNode->m_rt.w / 2.0f;
    h = pNode->m_rt.h / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x + w;
    y = pNode->m_rt.y;
    pNode->m_pChild[1] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x;
    y = pNode->m_rt.y + h;
    pNode->m_pChild[2] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x + w;
    y = pNode->m_rt.y + h;
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
    TNode* pNode = new TNode(pParent, x, y, w, h);
    return pNode;
}
// 1번 : 완전히 포함하는 노드에 추가하자.
// 2번 : 걸쳐만 있어도 노드에 추가하자.
void    TQuadtree::AddObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
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
                bool bIn = TCollision::RectToInRect(
                        pNode->m_pChild[iNode]->m_rt, 
                        pObj->m_rt);
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

std::vector<TObject*> TQuadtree::Collision(TObject* pObj)
{
    std::vector<TObject*> list;
    GetCollisitionObject(this->m_pRootNode, pObj, list);
    return list;
};
void  TQuadtree::GetCollisitionObject(TNode* pNode,
    TObject* pSrcObject,
    std::vector<TObject*>& list)
{
    if (pNode == nullptr) return;
    for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
    {
        if (TCollision::RectToRect(
            pNode->m_ObjectList[iObj]->m_rt, 
            pSrcObject->m_rt))
        {
            list.push_back(pNode->m_ObjectList[iObj]);
        }
    }
    if (pNode->m_pChild[0] == nullptr)
    {
        for (int iChild = 0; iChild < 4; iChild++)
        {
            if (TCollision::RectToRect(
                pNode->m_pChild[iChild]->m_rt, 
                pSrcObject->m_rt))
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
    player.SetPosition(50, 50, 20, 20);
    TQuadtree quadtree;
    quadtree.Create(100.0f, 100.0f);
    for (int iObj = 0; iObj < 100; iObj++)
    {
        TObject* pObj = new TObject;
        quadtree.AddObject(pObj);
    }
    while (1)
    {        
        std::vector<TObject*> list = quadtree.Collision(&player);
        std::cout << "player:"
            << player.m_rt.x << "," << player.m_rt.y << ","
            << player.m_rt.w << "," << player.m_rt.h
            << std::endl;
        if (!list.empty())
        {
            for (int iObj = 0; iObj < list.size(); iObj++)
            {
                std::cout << "object:"
                    << list[iObj]->m_rt.x << "," << list[iObj]->m_rt.y << ","
                    << list[iObj]->m_rt.w << "," << list[iObj]->m_rt.h
                    << std::endl;
            }
        }
        static float fDirectionX = 10.0f;
        if (rand() % 2 == 0)
        {
            fDirectionX *= -1.0f;
        }
        static float fDirectionY = 10.0f;
        if (rand() % 2 == 0)
        {
            fDirectionY *= -1.0f;
        }
        player.m_rt.x = player.m_rt.x + fDirectionX;
        player.m_rt.y = player.m_rt.y + fDirectionY;

        player.m_rt.x = min(player.m_rt.x, 100.0f);
        player.m_rt.x = max(player.m_rt.x, 0);
        player.m_rt.y = min(player.m_rt.y, 100.0f);
        player.m_rt.y = max(player.m_rt.y, 0);

        player.SetPosition( player.m_rt.x, 
                            player.m_rt.y, 30, 30);
        Sleep(1000);
        system("cls");
    }
    //화면좌표계(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
    //0,0 -> x       50,0         100(x), 0(y)        
    //|    0                  1
    //y 0,50         50,50        100,50   
    //     2                  3 
    //0,100          50,100       100,100 
    std::cout << "Hello World!\n";
}
