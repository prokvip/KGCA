#include "TQuadtree.h"
#include "TTimer.h"
std::vector<TObject*> StaticObjectList;
std::vector<TObject*> DynamicObjectList;

void SetStaticObject(TQuadtree& quadtree)
{    
    for (int i = 0; i < 3; i++)
    {
        TObject* obj = new TStaticObject();
        obj->m_csName = L"StaticObj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = { (float)(rand() % 800), (float)(rand() % 600) };
        quadtree.AddObject(obj);
        StaticObjectList.push_back(obj);
    }
}
void SetDynamicObject(TQuadtree& quadtree)
{
    for (int i = 0; i < 3; i++)
    {
        TObject* obj = new TDynamicObject();
        
        obj->m_csName = L"DynamicObj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = { (float)(rand() % 800), (float)(rand() % 600) };
        TPoint d = {  (float)randstep(0, 0), 
                     (float)randstep(0, 0) };
        obj->SetTarget(d);
        quadtree.AddObject(obj);
        DynamicObjectList.push_back(obj);
    }
}
int main()
{    
    TQuadtree quadtree;
    quadtree.BuildQuadtree();   
    SetStaticObject(quadtree);
    SetDynamicObject(quadtree);

    TTimer  timer;
    timer.Init();
    // 프레임 고정 100프레임    float fStep = 1.0f / 50.0f;
    while (timer.m_fGameTimer < 10.0f)
    {
        timer.Frame();       
        for (int i = 0; i < DynamicObjectList.size(); i++)
        {
            TObject* obj = DynamicObjectList[i];
            obj->Move(timer.m_fSecondPerFrame);
        }
        system("cls");
        std::cout << std::endl;
        std::cout << "Object inform!\n";
        quadtree.LevelOrder(quadtree.GetRootNode());
        timer.Render();
        //Sleep(1); // tick 1000 => 1초
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
    
}
