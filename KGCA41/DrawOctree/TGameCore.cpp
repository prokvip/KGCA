#include "TGameCore.h"
//È­¸éÁÂÇ¥°è(x1,y1,w,h) <-> p1(x1,y1)----- p2(x2,x2)
   //0,0 -> x       50,0         100(x), 0(y)        
   //|    0                  1
   //y 0,50         50,50        100,50   
   //     2                  3 
   //0,100          50,100       100,100 
template<class T>
bool TGameCore<T>::Init()
{ 
    for (int iObj = 0; iObj < 10; iObj++)
    {
        std::string name = std::to_string(iObj);
        name += " Static";
        TBaseObject* pObj = m_pWorldSP->NewStaticObject(name);
        m_AllObjectList.insert(std::make_pair(iObj, pObj));
        m_pWorldSP->AddStaticObject(pObj);
    }
    for (int iObj = 0; iObj < 10; iObj++)
    {
        std::string name = std::to_string(iObj);
        name += " Dynamic";
        TBaseObject* pObj = m_pWorldSP->NewDynamicObject(name);
        m_npcList.insert(std::make_pair(iObj, pObj));
        m_AllObjectList.insert(std::make_pair(10+iObj, pObj));
        m_pWorldSP->AddDynamicObject(pObj);
    }
    return true;
}
template<class T>
bool TGameCore<T>::Frame(float fDeltaTime, float fGameTime)
{
    m_pWorldSP->DynamicObjectReset();
    for (auto obj : m_npcList)
    {
        TBaseObject* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        m_pWorldSP->AddDynamicObject(pObject);
    }
    m_pPlayer->Frame(fDeltaTime, fGameTime);
    m_DrawList = m_pWorldSP->CollisionQuery(m_pPlayer->GetImp());
    return false;
}
template<class T>
bool TGameCore<T>::Render()
{
    m_pPlayer->Render();
    if (!m_DrawList.empty())
    {
        for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
        {
            m_DrawList[iObj]->Render();
        }
    }
    return false;
}
template<class T>
bool TGameCore<T>::Release()
{
    for (auto object : m_AllObjectList)
    {
        delete object.second;
    }
    m_AllObjectList.clear();
    m_npcList.clear();
    return true;
}
template<class T>
bool TGameCore<T>::Run()
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