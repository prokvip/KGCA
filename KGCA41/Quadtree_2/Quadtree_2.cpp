#include <windows.h>
#include <map>
#include "TCollision.h"


class TObject
{
public:
    TVector2D m_vDirection;
    TRect   m_rt;
    TCircle m_Circle;
    void   SetCircle(float x, float y, float w, float h)
    {
        m_Circle.cx = x;
        m_Circle.cy = y;
        float x1 = w/2.0f;
        float y1 = h/2.0f;
        m_Circle.fRadius = sqrt(x1 * x1 + y1 * y1);
    }
    void   SetPosition(float x, float y, float w, float h)
    {
        m_rt.Set(x,y,w,h);
        SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
    }
    void   Frame()
    {
        TVector2D vPos(m_rt.x1, m_rt.y1);
        // 벡터의 직전의 방정식
       //  결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간) 

        TVector2D vTmp = m_vDirection * 1.0f;
        vPos = vPos + vTmp;
        m_rt.x1 = vPos.x;
        m_rt.y1 = vPos.y;

        if (vPos.x > 100.0f)
        {
            m_rt.x1 = 100.0f;
            m_vDirection.x *= -1.0f;
        }
        if (vPos.x < 0.0f)
        {
            m_rt.x1 = 0.0f;
            m_vDirection.x *= -1.0f;
        }
        if (vPos.y > 100.0f)
        {
            m_rt.y1 = 100.0f;
            m_vDirection.y *= -1.0f;
        }
        if (vPos.y < 0.0f)
        {
            m_rt.y1 = 0.0f;
            m_vDirection.y *= -1.0f;
        }
        SetPosition(m_rt.x1, m_rt.y1, 30, 30);
    }
    TObject()
    {
        m_rt.Set(   20 + (rand() % 80), 
                    20 + (rand() % 80),
                    5.0f + (rand() % 5),
                    5.0f + (rand() % 5));
        SetCircle(m_rt.cx, m_rt.cy, m_rt.w, m_rt.h);
        m_vDirection.x = cos(rand());
        m_vDirection.y = sin(rand());
        m_vDirection.Normalized();
    }
};
class TNode
{
public:
    TRect m_rt;
    int    m_iDepth;
    std::vector<TObject*>  m_ObjectStaticList;
    std::vector<TObject*>  m_ObjectDynamicList;
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
    void    AddStaticObject(TObject* pObj);
    void    AddDynamicObject(TObject* pObj);
    TNode* FindNode(TNode* pNode, TObject* pObj);
    void    GetCollisitionObject(TNode* pNode,
        TObject* obj,
        std::vector<TObject*>& list);

    void   DynamicObjectReset(TNode* pNode);
    ~TQuadtree()
    {
        delete m_pRootNode;
    }
};
void   TQuadtree::DynamicObjectReset(TNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_ObjectDynamicList.clear();
    DynamicObjectReset(pNode->m_pChild[0]);
    DynamicObjectReset(pNode->m_pChild[1]);
    DynamicObjectReset(pNode->m_pChild[2]);
    DynamicObjectReset(pNode->m_pChild[3]);
}
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
    x = pNode->m_rt.x1;
    y = pNode->m_rt.y1;
    w = pNode->m_rt.w / 2.0f;
    h = pNode->m_rt.h / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1 + w;
    y = pNode->m_rt.y1;
    pNode->m_pChild[1] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1;
    y = pNode->m_rt.y1 + h;
    pNode->m_pChild[2] = CreateNode(pNode, x, y, w, h);
    x = pNode->m_rt.x1 + w;
    y = pNode->m_rt.y1 + h;
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
void    TQuadtree::AddStaticObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectStaticList.push_back(pObj);
    }
}
void    TQuadtree::AddDynamicObject(TObject* pObj)
{
    TNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_ObjectDynamicList.push_back(pObj);
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
    for (int iObj = 0; iObj < pNode->m_ObjectStaticList.size(); iObj++)
    {
        if (TCollision::RectToRect(
            pNode->m_ObjectStaticList[iObj]->m_rt,
            pSrcObject->m_rt))
        {
            list.push_back(pNode->m_ObjectStaticList[iObj]);
        }
    }
    for (int iObj = 0; iObj < pNode->m_ObjectDynamicList.size(); iObj++)
    {
        if (TCollision::CircleToCircle(
            pNode->m_ObjectDynamicList[iObj]->m_Circle,
            pSrcObject->m_Circle))
        {
            list.push_back(pNode->m_ObjectDynamicList[iObj]);
        }        
    }
    if (pNode->m_pChild[0] != nullptr)
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
    float fRadian = DegreeToRadian(180);
    float fDegree = RadianToDegree(fRadian);
    TVector2D va(800,0);
    TVector2D vb(800, 800);
    //TVector2D vaa = va.Identity();
    //TVector2D vbb = vb.Identity();    
    float fAngle = va.Angle(vb);
    TVector2D vc = va + vb;
    TVector2D vd = va - vb;
    float  fA = va.Length();
    float  fB = vb.Length();

    TVector2D vf(2, 2);
    float  fF = vf.Length();

    TVector2D vIdentityA = va.Identity();
    TVector2D vIdentityB = vb.Identity();

    float fLengthA = vIdentityA.Length();
    float fLengthB = vIdentityB.Length();

    TVector2D vaA = vIdentityA * fA;
    TVector2D vaB = vIdentityB * fB;

    vaA.Normalized();
    vaB.Normalized();
    if (va == vb)
    {

    }
    //std::vector<int> list1 = { 4,5,64, };
    //std::vector<int> list2;
    //list2 = list1;

    TObject player;
    player.SetPosition(0, 100, 10, 10);
    TQuadtree quadtree;
    quadtree.Create(100.0f, 100.0f);
    float fDistance = 0.0f;

    std::map<int, TObject*> objList;
    std::map<int, TObject*> AllobjList;
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        AllobjList.insert(std::make_pair(iObj, pObj));
        quadtree.AddStaticObject(pObj);        
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        objList.insert(std::make_pair(iObj, pObj));
        AllobjList.insert(std::make_pair(iObj, pObj));
        quadtree.AddDynamicObject(pObj);
    }
 /*   TObject* pObj = new TObject;
    pObj->SetPosition(100, 0, 20, 20);
    quadtree.AddObject(pObj);

    if (TCollision::CircleToCircle(
        player.m_Circle,
        pObj->m_Circle))
    {

    }*/

    //player.m_vDirection.x = 1.0f;
    //player.m_vDirection.y = 0.0f;

    while (1)
    {   
        quadtree.DynamicObjectReset(quadtree.m_pRootNode);
        for (auto obj : objList)
        {
            TObject* pObject = obj.second;
            pObject->Frame();
            quadtree.AddDynamicObject(pObject);
        }
        TVector2D vPos(player.m_rt.x1, player.m_rt.y1);
        // 벡터의 직전의 방정식
       //  결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간) 
        TVector2D vTmp = player.m_vDirection * 1.0f;
        vPos = vPos + vTmp;
        player.m_rt.x1 = vPos.x;
        player.m_rt.y1 = vPos.y;

        if (vPos.x > 100.0f)
        {
            player.m_rt.x1 = 100.0f;
            player.m_vDirection.x *= -1.0f;
        }
        if (vPos.x < 0.0f)
        {
            player.m_rt.x1 = 0.0f;
            player.m_vDirection.x *= -1.0f;
        }
        if (vPos.y > 100.0f)
        {
            player.m_rt.y1 = 100.0f;
            player.m_vDirection.y *= -1.0f;
        }
        if (vPos.y < 0.0f)
        {
            player.m_rt.y1 = 0.0f;
            player.m_vDirection.y *= -1.0f;
        }
        player.SetPosition(player.m_rt.x1, player.m_rt.y1, 30, 30);


        std::vector<TObject*> list = quadtree.Collision(&player);
        std::cout << "player:"
            << player.m_rt.x1 << "," << player.m_rt.y1 << ","
            << player.m_rt.x2 << "," << player.m_rt.y2
            << std::endl;
        if (!list.empty())
        {
            for (int iObj = 0; iObj < list.size(); iObj++)
            {
                std::cout << "object:"
                    << list[iObj]->m_rt.x1 << "," << list[iObj]->m_rt.y1 << ","
                    << list[iObj]->m_rt.x2 << "," << list[iObj]->m_rt.y2
                    << std::endl;
            }
        }             
        
        Sleep(100);
        system("cls");
    }

    for (auto object : AllobjList)
    {
        delete object.second;
    }
    AllobjList.clear();
    objList.clear();
    //화면좌표계(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
    //0,0 -> x       50,0         100(x), 0(y)        
    //|    0                  1
    //y 0,50         50,50        100,50   
    //     2                  3 
    //0,100          50,100       100,100 
    std::cout << "Hello World!\n";
}
