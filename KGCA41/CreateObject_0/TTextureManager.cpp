#include "TTextureManager.h"
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
    // �ߺ�����
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    TTexture* pNewData = new TTexture;
    if (pNewData)
    {
        hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, name);
        if (SUCCEEDED(hr))
        {
            m_List.insert(std::make_pair(name, pNewData));
        }
    }
	return pNewData;
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