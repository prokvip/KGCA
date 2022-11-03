#include "TTextureManager.h"
W_STR TTextureManager::GetSplitName(std::wstring fullpath)
{
    W_STR name;
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(fullpath.c_str(),
        dirve, dir, filename, ext);
    name = filename;
    name += ext;
    return name;
}
W_STR TTextureManager::GetSplitName(std::string fullpath)
{
    W_STR szUnicode = to_mw(fullpath);
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(szUnicode.c_str(), dirve, dir, filename, ext);
    W_STR fileName = filename;
    W_STR fileExt = ext;
    if (fileExt == L".tga" || fileExt == L".TGA")
    {
        fileExt = L".dds";
    }
    fileName += fileExt;
    return fileName;
}
void TTextureManager::SetDevice(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}
TTexture* TTextureManager::Load(std::wstring name)
{
    HRESULT hr;
    // 중복제거
    auto data = Find(name);
    if (data != nullptr) return data;

    TTexture* pNewData = new TTexture;
    if (pNewData)
    {
        hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, name);
        if (SUCCEEDED(hr))
        {
            m_List.insert(std::make_pair(name, pNewData));
            return pNewData;
        }
    }
    delete pNewData;
	return nullptr;
}
TTexture* TTextureManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    return nullptr;
}
TTextureManager::TTextureManager()
{
}
bool TTextureManager::Release()
{
    for (auto data : m_List)
    {
        TTexture* pTexture = data.second;
        if (pTexture) pTexture->Release();
        delete pTexture;
    }
    m_List.clear();
    return true;
}
TTextureManager::~TTextureManager()
{
    Release();
}