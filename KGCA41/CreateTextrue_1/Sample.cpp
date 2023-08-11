#include "Sample.h"
bool  Sample::Init() 
{   
    std::wstring texname[3] = { L"kgcabk.bmp" ,L"ade4.dds" ,L"bitmap1.bmp" };
    for (int iObj = 0; iObj < 300; iObj++)
    {
        TObject* pObj = new TObject;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->Create(texname[iObj%3]);
        m_ObjList.push_back(pObj);
    }
    return true; 
}
bool  Sample::Frame() 
{ 
    for (auto obj : m_ObjList)
    {
        obj->Frame();
    }
    return true; 
}
bool  Sample::Render() 
{    
    for (auto obj : m_ObjList)
    {
        obj->Render();
    }
    return true; 
}
bool  Sample::Release()
{   
    for (auto obj : m_ObjList)
    {
        obj->Release();
        delete obj;
    }
    m_ObjList.clear();
    return true; 
}

TGAME(L"kgca", 800, 600)