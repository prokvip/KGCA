#include "TSoundManager.h"
bool TSoundManager::Init()
{
    FMOD::System_Create(&m_pSystem);
    m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
    return true;
}
TSound* TSoundManager::Load(std::wstring name)
{
    HRESULT hr;
    // 중복제거
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    TSound* pNewData = new TSound;
    if (pNewData)
    {
        hr = pNewData->Load(m_pSystem,name);
        if (SUCCEEDED(hr))
        {
            m_List.insert(std::make_pair(name, pNewData));
        }
    }
    return pNewData;
}
bool TSoundManager::Frame()
{
    m_pSystem->update();
    for (auto data : m_List)
    {
        TSound* pData = data.second;
        if (pData) pData->Frame();
    }
    return true;
}
TSoundManager::TSoundManager()
{
}
bool TSoundManager::Release()
{
    for (auto data : m_List)
    {
        TSound* pData = data.second;
        if (pData) pData->Release();
        delete pData;
    }
    m_pSystem->close();
    m_pSystem->release();
    m_List.clear();
    return true;
}
TSoundManager::~TSoundManager()
{
    Release();
}