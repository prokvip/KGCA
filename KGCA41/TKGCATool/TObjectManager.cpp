#include "pch.h"
#include "TObjectManager.h"
W_STR TObjectManager::GetSplitName(std::wstring fullpath)
{
    W_STR name;
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(fullpath.c_str(), dirve, dir, filename, ext);
    name = filename;
    name += ext;
    return name;
}
void TObjectManager::SetDevice(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}
TFbxFile* TObjectManager::LoadFbx(std::wstring name)
{
    // 중복제거
    auto data = FindFbx(name);
    if (data != nullptr)
    {
        return data;
    }

    TFbxFile* pFbxFile = new TFbxFile;
    pFbxFile->m_szName = GetSplitName(name);
    if (pFbxFile)
    {
        if (pFbxFile->Init())
        {
            if (pFbxFile->Load(to_wm(name)))
            {
                pFbxFile->CreateConstantBuffer(m_pd3dDevice);
                m_fbxList.insert(std::make_pair(name, pFbxFile));
                return pFbxFile;
            }
            else
            {
                delete pFbxFile;
                pFbxFile = nullptr;
            }
        }
    }
    return pFbxFile;
}

TCharacter* TObjectManager::Load(std::wstring fbxfilepath)
{
    HRESULT hr;
    auto obj = FindCharacter(fbxfilepath);
    if (obj != nullptr)
    {
        return obj;
    }
    // 중복제거
    auto data = LoadFbx(fbxfilepath);
    if (data != nullptr)
    {
        TCharacter* pNewData = new TCharacter;
        pNewData->m_szName = GetSplitName(fbxfilepath);
        if (pNewData)
        {
            hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, data);
            if (SUCCEEDED(hr))
            {
                m_List.insert(std::make_pair(pNewData->m_szName, pNewData));
                return pNewData;
            }
            else
            {
                delete pNewData;
            }
        }       
    } 
	return nullptr;
}
TCharacter* TObjectManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    return nullptr;
}
TFbxFile* TObjectManager::FindFbx(std::wstring name)
{
    auto iter = m_fbxList.find(GetSplitName(name));
    if (iter != m_fbxList.end())
    {
        return iter->second;
    }
    return nullptr;
}
TCharacter* TObjectManager::FindCharacter(std::wstring name)
{
    auto iter = m_List.find(GetSplitName(name));
    if (iter != m_List.end())
    {
        return iter->second;
    }
    return nullptr;
}
TObjectManager::TObjectManager()
{
}
bool TObjectManager::Release()
{
    for (auto data : m_fbxList)
    {
        TFbxFile* pData = data.second;
        if (pData) pData->Release();
        delete pData;
        pData = nullptr;
    }
    m_fbxList.clear();
    for (auto data : m_List)
    {
        TCharacter* pData = data.second;
        if (pData) pData->Release();
        delete pData;
        pData = nullptr;
    }
    m_List.clear();
    return true;
}
TObjectManager::~TObjectManager()
{
    Release();
}