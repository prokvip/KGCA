#include "TQuadtree.h"
#include "TOctree.h"
#include "TTimer.h"
#include <map> // key+value   bst
#include <unordered_map> 
#include <queue>
using MyList = std::unordered_map<int, TObject3D*>;
MyList StaticObjectList;
MyList DynamicObjectList;
//std::vector<TObject3D*> StaticObjectList;
//std::vector<TObject3D*> DynamicObjectList;

void SetStaticObject(TOctree& tree)
{    
    if (tree.m_pRootNode == nullptr) return;

    int iMaxWidth = tree.m_pRootNode->m_tBox.m_fWidth;
    int iMaxHeight = tree.m_pRootNode->m_tBox.m_fHeight;
    int iMaxDepth = tree.m_pRootNode->m_tBox.m_fDepth;
    for (int i = 0; i < 1000; i++)
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
        StaticObjectList.insert(std::make_pair(i,obj));
    }
}
void SetDynamicObject(TOctree& tree)
{
    if (tree.m_pRootNode == nullptr) return;
    int iMaxWidth = tree.m_pRootNode->m_tBox.m_fWidth;
    int iMaxHeight = tree.m_pRootNode->m_tBox.m_fHeight;
    int iMaxDepth = tree.m_pRootNode->m_tBox.m_fDepth;

    for (int i = 0; i < 100000; i++)
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
        DynamicObjectList.insert(std::make_pair(i, obj));        
    }
}
//struct TData
//{
//    TData* pNext;
//};

auto  Fun1()
{
    return std::pair<int, std::string>(1,"a");
}
auto  Fun2()
{
    return std::tuple<int, std::string,float>(1, "aaa", 3.14f);
}
auto  Fun3()
{
    return std::vector<int>(10);
}
struct cmp2
{
    bool operator()(int a, int b)
    {
        return a > b;
    }
};
int main()
{ 
    std::priority_queue<int> qList1;
    for (int n : {3, 6, 23, 6, 7, 23, 7, 32, 5, 3, 6, 23, 643, 6345, 3})
    {
        qList1.push(n);
    }

    while (!qList1.empty())
    {        
        qList1.push(rand() % 1000);
        std::cout << qList1.top() << " ";
        qList1.pop();
        std::cout << qList1.top() << " ";
        qList1.pop();       
    }
    auto cmp = [](int a, int b)->bool { return a < b; };
    //std::priority_queue<int, std::vector<int>, cmp2 > qList2;
    //std::priority_queue<int, std::vector<int>, std::greater<int> > qList2;
    std::priority_queue<int, std::vector<int>, decltype(cmp) > qList2(cmp);
    for (int n : {3, 6, 23, 6, 7, 23, 7, 32, 5, 3, 6, 23, 643, 6345, 3})
    {
        qList2.push(n);
    }

    while (!qList2.empty())
    {
        std::cout << qList2.top() << " ";
        qList2.pop();        
    }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    std::pair<int, std::string> r1 = Fun1();
    std::tuple<int, std::string, float> r2 = Fun2();
    std::vector<int> ret = Fun3();
    
    int iID;
    std::string name;
    std::tie(iID, name) = Fun1();   


    using MyMap = std::map<int, std::string>;
    std::pair<int, std::string> pairdata;
    MyMap mapList1;
    pairdata.first = 67;
    pairdata.second = "혹길";
    mapList1.insert(pairdata);

    pairdata.first = 9;
    pairdata.second = "강시";
    mapList1.insert(pairdata);

    //pairdata.first = 23;
    //pairdata.second = "강3시";
    mapList1.insert(std::make_pair(23, "강3시"));
    mapList1.insert(std::make_pair(23, "강5시"));
    /*mapList1[67] = "혹길";
    mapList1[9] = "강시";
    mapList1[23] = "강3시";
    mapList1[67] = "2혹길";*/
    MyMap::iterator d1 = mapList1.find(9);
    
    auto d3 = mapList1.find(19);
    if (d3 == mapList1.end())
    {
        
    }
  
    for (std::pair<int, std::string> d2 : mapList1)
    {
        int key = d2.first;
        std::string value = d2.second;
    }
    for (auto& d7 : mapList1)
    {
        int key = d7.first;
        std::string value = d7.second;
    }
    for (MyMap::iterator d3 = mapList1.begin();
         d3 != mapList1.end();
         d3++)
    {
     
    }
    for (int iKey = 0; iKey < mapList1.size(); iKey++)
    {
        std::cout << mapList1[iKey].c_str();
    }
    

    //TData* pa = new TData;
    //pa->pNext = nullptr;
    //TData* pb = new TData;
    //pb->pNext = nullptr;
    //TData* pc = new TData;
    //pc->pNext = nullptr;
    //pa->pNext = pb;
    //pb->pNext = pc;
    //for (TData* pData = pa;
    //    pData != nullptr;
    //    pData = pData->pNext)
    //{
    //    std::cout << pData << std::endl;
    //}
    //delete pa;
    //delete pb;
    //delete pc;

#pragma region ->사이각 반환(180내의 작은값)
    /// <summary>
    /// 사이각 반환(180내의 작은값)
    /// </summary>   
    TVector3  x(1, 0, 0);
    TVector3  z(0, 0, 1);
    TVector3  y = (z ^ x);// up
    float fDot1 = y | x;
    float fDot2 = y | z;
    float fDegree = z.Angle(x);
#pragma endregion <-사이각 반환(180내의 작은값)
#pragma region ->직선과 한점의 거리 반환
    /// <summary>
    /// 직선과 한점의 거리 반환
    /// </summary>
    /// <returns></returns>
    TRay ray;
    ray.vPosition = { 0,0,0 };
    ray.vDirection = { 10000,0,0 };
    TVector3  p0(10, 0, -7);
    float fDistance = ray.GetDistance(p0);
#pragma endregion 
#pragma region ->평면과 한점의 거리
    /// <summary>
    /// 평면과 한점의 거리
    /// </summary>
    /// <returns></returns>
    TPlane plane1(TVector3(10, -10, 0),   TVector3(1,0,0));
    TPlane plane2(TVector3(0, -10, 0), TVector3(0, -10, 10), TVector3(10, -10, 0));
    TVector3 p1(20, 30, 40);
    float f0 = plane1.GetDistance(p1);
    float f1 = plane2.GetDistance(p1);   
#pragma endregion 
#pragma region ->임의의 벡터를 기준으로 360로 반환한다
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
#pragma endregion 


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
    //while (timer.m_fGameTimer < 1000.0f)
    while(1)
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

        //system("cls");
        std::cout << std::endl;
        std::cout << "Object inform!\n";
        for (int iObj=0; iObj < DynamicObjectList.size(); iObj++ )
        {
            std::cout << "Object[" << iObj << "]" << 
                DynamicObjectList[iObj]->m_iNodeIndex << std::endl;
        }
        //tree.LevelOrder(tree.m_pRootNode);
        timer.Render();
        //Sleep(100); // tick 1000 => 1초
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
