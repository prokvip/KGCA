#include "TQuadtree.h"
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

    int iMaxFrame = 30;   
    while (--iMaxFrame > 0)
    {
        for (int i = 0; i < DynamicObjectList.size(); i++)
        {
            TObject* obj = DynamicObjectList[i];
            obj->Move();
        }
        system("cls");
        std::cout << std::endl;
        std::cout << "[" << iMaxFrame <<"]" << "Object inform!\n";
        quadtree.LevelOrder(quadtree.GetRootNode());
        std::cout << "Hello World!\n";
        Sleep(1000); // tick 1000 => 1초
    }

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
