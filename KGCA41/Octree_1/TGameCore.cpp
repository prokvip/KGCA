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
        TObject* pObj = new TObject;
        m_AllObjectList.insert(std::make_pair(iObj, pObj));
        m_pWorldSP->AddStaticObject(pObj);
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        m_npcList.insert(std::make_pair(iObj, pObj));
        m_AllObjectList.insert(std::make_pair(iObj, pObj));
        m_pWorldSP->AddDynamicObject(pObj);
    }
    return true;
}

bool TGameCore::Frame(float fDeltaTime, float fGameTime)
{
   /* m_pWorldSP->DynamicObjectReset(m_pWorldSP->m_pRootNode);
    for (auto obj : m_npcList)
    {
        TObject* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        m_pWorldSP->AddDynamicObject(pObject);
    }
    m_Player2D.Frame(fDeltaTime, fGameTime);

    m_DrawList = m_pWorldSP->Collision(&m_Player2D);*/
    
    return false;
}

bool TGameCore::Render()
{
    /*std::cout << "m_Player2D:"
        << m_Player2D.m_Box.x1 << "," << m_Player2D.m_Box.y1 << ","
        << m_Player2D.m_Box.x2 << "," << m_Player2D.m_Box.y2
        << std::endl;
    if (!m_DrawList.empty())
    {
        for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
        {
            std::cout << "object:"
                << m_DrawList[iObj]->m_Box.x1 << "," << m_DrawList[iObj]->m_Box.y1 << ","
                << m_DrawList[iObj]->m_Box.x2 << "," << m_DrawList[iObj]->m_Box.y2
                << std::endl;
        }
    }*/
    return false;
}

bool TGameCore::Release()
{
  /*  for (auto object : m_AllObjectList)
    {
        delete object.second;
    }
    m_AllObjectList.clear();
    m_npcList.clear();*/
    return false;
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