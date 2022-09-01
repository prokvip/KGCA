#include "Sample.h"
#include "TTextureManager.h"
#include "TShaderManager.h"
bool	Sample::Init()
{     
    I_Tex.SetDevice(m_pd3dDevice, m_pImmediateContext);
    I_Shader.SetDevice(m_pd3dDevice, m_pImmediateContext);
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TBaseObject* pObject = new TBaseObject;
        pObject->Create(m_pd3dDevice, m_pImmediateContext,
            L"../../data/kgcabk.bmp",
            L"../../data/shader/DefaultShape.txt");
        m_pObjectList.push_back(pObject);
    }
    return true;
}
bool		Sample::Frame()
{
    for (auto obj : m_pObjectList)
    {
        obj->Frame();
    }
    return true;
}
bool		Sample::Render()
{
    for (auto obj : m_pObjectList)
    {
        obj->Render();
    }
    return true;
}
bool		Sample::Release()
{
    for (auto obj : m_pObjectList)
    {
        obj->Release();
        delete obj;
    }
    return true;
}


GAME_RUN(CreateObject_0, 1024, 768)
