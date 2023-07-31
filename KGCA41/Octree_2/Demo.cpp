#include "TQuadtree.h"
#include "TOctree.h"
#include "TTimer.h"
std::vector<TObject3D*> StaticObjectList;
std::vector<TObject3D*> DynamicObjectList;

void SetStaticObject(TOctree& tree)
{    
    if (tree.m_pRootNode == nullptr) return;

    int iMaxWidth = tree.m_pRootNode->m_tBox.m_fWidth;
    int iMaxHeight = tree.m_pRootNode->m_tBox.m_fHeight;
    int iMaxDepth = tree.m_pRootNode->m_tBox.m_fDepth;
    for (int i = 0; i < 10; i++)
    {
        TObject3D* obj = new TStaticObject3D();
        obj->m_csName = L"StaticObj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = { (float)(rand() % iMaxWidth),
                            (float)(rand() % iMaxHeight),
                            (float)(rand() % iMaxDepth) };
        obj->SetBox(obj->m_Position, 
                    (float)((rand() % 10) + 1.0f), 
                    (float)((rand() % 10) + 1.0f),
                    (float)((rand() % 10) + 1.0f));
        
        tree.StaticAddObject(obj);
        StaticObjectList.push_back(obj);
    }
}
void SetDynamicObject(TOctree& tree)
{
    if (tree.m_pRootNode == nullptr) return;
    int iMaxWidth = tree.m_pRootNode->m_tBox.m_fWidth;
    int iMaxHeight = tree.m_pRootNode->m_tBox.m_fHeight;
    int iMaxDepth = tree.m_pRootNode->m_tBox.m_fDepth;

    for (int i = 0; i < 10; i++)
    {
        TObject3D* obj = new TDynamicObject3D();
        
        obj->m_csName = L"DynamicObj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = {
            (float)(rand() % iMaxWidth), 
            (float)(rand() % iMaxHeight),
            (float)(rand() % iMaxDepth) };
        obj->SetBox(obj->m_Position,
            (float)((rand() % 10) + 1.0f),
            (float)((rand() % 10) + 1.0f),
            (float)((rand() % 10) + 1.0f));

        TVector3 d = {   (float)randstep(0, iMaxWidth),
                        (float)randstep(0, iMaxHeight),
                        (float)randstep(0, iMaxDepth) };
        obj->SetTarget(d);
        tree.DynamicAddObject(obj);
        DynamicObjectList.push_back(obj);
    }
}
int main()
{ 
    /// <summary>
    /// 사이각 반환(180내의 작은값)
    /// </summary>   
    TVector3  x(1, 0, 0);
    TVector3  z(0, 0, 1);
    TVector3  y = (z ^ x);// up
    float fDot1 = y | x;
    float fDot2 = y | z;
    float fDegree = z.Angle(x);
    /// <summary>
    /// 직선과 한점의 거리 반환
    /// </summary>
    /// <returns></returns>
    TRay ray;
    ray.vPosition = { 0,0,0 };
    ray.vDirection = { 10000,0,0 };
    TVector3  p0(10, 0, -7);
    float fDistance = ray.GetDistance(p0);
    /// <summary>
    /// 평면과 한점의 거리
    /// </summary>
    /// <returns></returns>
    TPlane plane1(TVector3(10, -10, 0),   TVector3(1,0,0));
    TPlane plane2(TVector3(0, -10, 0), TVector3(0, -10, 10), TVector3(10, -10, 0));
    TVector3 p1(20, 30, 40);
    float f0 = plane1.GetDistance(p1);
    float f1 = plane2.GetDistance(p1);    
    /// <summary>
    /// 임의의 벡터를 기준으로 360로 반환한다.
    /// </summary>
    /// <returns></returns>
    TVector3  v(10, 0, -10);
    v.Normalize();
    TVector3  up = (z ^ v);
    up.Normalize();
    float fDot3 = up | y;
    float fDegree360 = 0.0f;
    if (fDot3 > 0.0f)
    {
        fDegree360 = z.Angle(v);
    }
    else
    {
        fDegree360 = (180.0f - z.Angle(v)) + 180.0f;
    }



    TOctree tree;
    TSpaceData data;
    data.p3 = { 0,0,0 };
    data.w = 800;
    data.h = 600;
    data.z = 600;
    
    tree.BuildOctTree(data);

    SetStaticObject(tree);
    SetDynamicObject(tree);

    TTimer  timer;
    timer.Init();
    // 프레임 고정 100프레임    float fStep = 1.0f / 50.0f;
    while (timer.m_fGameTimer < 1000.0f)
    {
        timer.Frame();       
        tree.PreFrame();

        for (int i = 0; i < DynamicObjectList.size(); i++)
        {
            TObject3D* obj = DynamicObjectList[i];
            obj->Move(timer.m_fSecondPerFrame);
            tree.DynamicAddObject(obj);
        }
        
        tree.Frame();
        tree.Render();

        system("cls");
        std::cout << std::endl;
        std::cout << "Object inform!\n";
        for (int iObj=0; iObj < DynamicObjectList.size(); iObj++ )
        {
            std::cout << "Object[" << iObj << "]" << 
                DynamicObjectList[iObj]->m_iNodeIndex << std::endl;
        }
        //tree.LevelOrder(tree.m_pRootNode);
        timer.Render();
        Sleep(100); // tick 1000 => 1초
    }
    timer.Release();

    for (int i = 0; i < StaticObjectList.size(); i++)
    {
        delete StaticObjectList[i];
    }
    StaticObjectList.clear();
    for (int i = 0; i < DynamicObjectList.size(); i++)
    {
        delete DynamicObjectList[i];
    }
    DynamicObjectList.clear();

    tree.Release();
    
}
