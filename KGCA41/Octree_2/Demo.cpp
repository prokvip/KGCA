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
    TVector3  x(1, 0, 0);
    TVector3  z(0, 0, 1);
    TVector3  y = (z ^ x);
    float fDot1 = y | x;
    float fDot2 = y | z;

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
