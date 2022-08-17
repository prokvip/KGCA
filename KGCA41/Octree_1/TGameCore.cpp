#include "TGameCore.h"
//È­¸éÁÂÇ¥°è(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
   //0,0 -> x       50,0         100(x), 0(y)        
   //|    0                  1
   //y 0,50         50,50        100,50   
   //     2                  3 
   //0,100          50,100       100,100 
bool TGameCore::Init()
{    
    for (int iObj = 0; iObj < 10; iObj++)
    {
        std::string name = std::to_string(iObj);
        name += " Static";
        TObject* pObj = new TObject(name);
        m_AllObjectList.insert(std::make_pair(iObj, pObj));
        m_pWorldSP->AddStaticObject(pObj);
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        std::string name = std::to_string(iObj);
        name += " Dynamic";
        TObject* pObj = new TEnemy;
        pObj->m_csName = name;
        m_npcList.insert(std::make_pair(iObj, pObj));
        m_AllObjectList.insert(std::make_pair(10+iObj, pObj));
        m_pWorldSP->AddDynamicObject(pObj);
    }
    return true;
}

bool TGameCore::Frame(float fDeltaTime, float fGameTime)
{
    m_pWorldSP->DynamicObjectReset(m_pWorldSP->m_pRootNode);
    for (auto obj : m_npcList)
    {
        TObject* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        m_pWorldSP->AddDynamicObject(pObject);
    }
    m_Player.Frame(fDeltaTime, fGameTime);
    m_DrawList = m_pWorldSP->CollisionQuery(&m_Player);
    return false;
}

bool TGameCore::Render()
{
    std::cout << "m_Player:"
        << m_Player.m_Box.vMin.x << "," << m_Player.m_Box.vMin.y << ","
        << m_Player.m_Box.vMin.z << std::endl;
    if (!m_DrawList.empty())
    {
        for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
        {
            std::cout << m_DrawList[iObj]->m_csName << ","
                << m_DrawList[iObj]->m_Box.vMin.x << "," 
                << m_DrawList[iObj]->m_Box.vMin.y << ","
                << m_DrawList[iObj]->m_Box.vMin.z << std::endl;
        }
    }
    return false;
}

bool TGameCore::Release()
{
    for (auto object : m_AllObjectList)
    {
        delete object.second;
    }
    m_AllObjectList.clear();
    m_npcList.clear();
    return true;
}

bool TGameCore::Run()
{
    Init();
    float  fGameTimer = 0.0f;
    float  fDelay = 10;
    while (fGameTimer < 10.0f)
    {
        Frame(fDelay/1000.0f, fGameTimer);
        Render();
        Sleep(fDelay);
        system("cls");
        fGameTimer += fDelay / 1000.0f;
    }
    Release();   
    return true;
}