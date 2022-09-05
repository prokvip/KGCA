#include "TShaderManager.h"
void TShaderManager::SetDevice(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}
TShader* TShaderManager::Load(std::wstring name)
{
    HRESULT hr;
    // �ߺ�����
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    TShader* pNewData = new TShader;
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

TShaderManager::TShaderManager()
{
}
bool TShaderManager::Release()
{
    for (auto data : m_List)
    {
        TShader* pData = data.second;
        if (pData) pData->Release();
        delete pData;
    }
    m_List.clear();
    return true;
}
TShaderManager::~TShaderManager()
{
    Release();
}