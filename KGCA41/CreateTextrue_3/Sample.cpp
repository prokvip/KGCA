#include "Sample.h"
bool  Sample::Init() 
{   
    m_texMgr.Set(m_pDevice, m_pImmediateContext);
    m_shaderMgr.Set(m_pDevice, m_pImmediateContext);

    std::wstring texname[] = { 
        L"../../res/kgcabk.bmp", // path=../../res/, name = kgcabk.bmp
        L"../../res/ade4.dds" ,
        L"../../res/mapcontrol.png",
        L"../../res/103.tga"
    };
    for (int iObj = 0; iObj < 100; iObj++)
    {
        TObject* pObj = new TObject;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->Create( m_texMgr, texname[iObj % 4],
                      m_shaderMgr, L"Plane.hlsl");
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