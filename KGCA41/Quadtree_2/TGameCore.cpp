#include "TGameCore.h"
//È­¸éÁÂÇ¥°è(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
   //0,0 -> x       50,0         100(x), 0(y)        
   //|    0                  1
   //y 0,50         50,50        100,50   
   //     2                  3 
   //0,100          50,100       100,100 
bool TGameCore::Init()
{
    player.SetPosition(0, 100, 10, 10);
    quadtree.Create(100.0f, 100.0f);
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TMapObject;
        AllObjectList.insert(std::make_pair(iObj, pObj));
        quadtree.AddStaticObject(pObj);
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TNonCharacter;
        npcList.insert(std::make_pair(iObj, pObj));
        AllObjectList.insert(std::make_pair(iObj, pObj));
        quadtree.AddDynamicObject(pObj);
    }
    return false;
}

bool TGameCore::Frame()
{
    quadtree.DynamicObjectReset(quadtree.m_pRootNode);
    for (auto obj : npcList)
    {
        TObject* pObject = obj.second;
        pObject->Frame();
        quadtree.AddDynamicObject(pObject);
    }
    player.Frame();

    DrawList = quadtree.Collision(&player);
    
    return false;
}

bool TGameCore::Render()
{
    std::cout << "player:"
        << player.m_rt.x1 << "," << player.m_rt.y1 << ","
        << player.m_rt.x2 << "," << player.m_rt.y2
        << std::endl;
    if (!DrawList.empty())
    {
        for (int iObj = 0; iObj < DrawList.size(); iObj++)
        {
            std::cout << "object:"
                << DrawList[iObj]->m_rt.x1 << "," << DrawList[iObj]->m_rt.y1 << ","
                << DrawList[iObj]->m_rt.x2 << "," << DrawList[iObj]->m_rt.y2
                << std::endl;
        }
    }
    return false;
}

bool TGameCore::Release()
{
    for (auto object : AllObjectList)
    {
        delete object.second;
    }
    AllObjectList.clear();
    npcList.clear();
    return false;
}

bool TGameCore::Run()
{
    Init();
    float  fGameTimer = 0.0f;
    while (fGameTimer < 10.0f)
    {
        Frame();
        Render();
        Sleep(100);
        system("cls");
        fGameTimer += 0.1f;
    }
    Release();   
    return true;
}