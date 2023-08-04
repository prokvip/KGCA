#include "TQuadtree.h"
#include "TOctree.h"
#include "TTimer.h"
#include <map> // key+value   bst
#include <unordered_map> 
#include <queue>
using MyList = std::unordered_map<int, TObject3D*>;
MyList StaticObjectList;
MyList DynamicObjectList;

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
        obj->m_Position = { (float)(rand() % iMaxWidth)- (iMaxWidth/2.0f),
                            (float)(rand() % iMaxHeight) - (iMaxHeight / 2.0f),
                            (float)(rand() % iMaxDepth) - (iMaxDepth / 2.0f) };
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

    for (int i = 0; i < 10; i++)
    {
        TObject3D* obj = new TDynamicObject3D();
        
        obj->m_csName = L"DynamicObj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = { (float)(rand() % iMaxWidth) - (iMaxWidth / 2.0f),
                           (float)(rand() % iMaxHeight) - (iMaxHeight / 2.0f),
                           (float)(rand() % iMaxDepth) - (iMaxDepth / 2.0f) };

        obj->SetBox(obj->m_Position,
            (float)((rand() % 10) + 1.0f),
            (float)((rand() % 10) + 1.0f),
            (float)((rand() % 10) + 1.0f));

        TVector3 d = {   (float)randstep(-iMaxWidth/2.0f, iMaxWidth/2.0f),
                        (float)randstep(-iMaxWidth / 2.0f, iMaxHeight / 2.0f),
                        (float)randstep(-iMaxWidth / 2.0f, iMaxDepth / 2.0f) };
        obj->SetTarget(d);
        tree.DynamicAddObject(obj);
        DynamicObjectList.insert(std::make_pair(i, obj));        
    }
}

int main()
{   
    TOctree tree;
    TSpaceData data;
    data.p3 = { 0.0f,0.0f, 0.0f };
    data.w = g_fMapSize;
    data.h = g_fMapSize;
    data.z = g_fMapSize;
    
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
