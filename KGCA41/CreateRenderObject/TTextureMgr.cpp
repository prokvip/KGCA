#include "TTextureMgr.h"
bool  TTexture::Release()
{
    if (m_pTexSRV)m_pTexSRV->Release();
    m_pTexSRV = nullptr;
    return true;
}
bool  TTexture::Load(ID3D11Device* pDevice, std::wstring filename)
{
    auto imageobj = std::make_unique<DirectX::ScratchImage>();
    DirectX::TexMetadata mdata;

    HRESULT hr = DirectX::GetMetadataFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(pDevice, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);
            if (SUCCEEDED(hr))
            {
                return true;
            }
        }
    }
    hr = DirectX::GetMetadataFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(pDevice, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);
            if (SUCCEEDED(hr))
            {
                return true;
            }
        }
    }
    hr = DirectX::GetMetadataFromTGAFile(filename.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromTGAFile(filename.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(pDevice, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);
            if (SUCCEEDED(hr))
            {
                return true;
            }
        }
    }
    return false;
}

void  TTextureMgr::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
    m_pDevice = pDevice;
    m_pImmediateContext = pImmediateContext;
}
const TTexture* TTextureMgr::Load(std::wstring szPullfilePath)
{
    std::size_t found = szPullfilePath.find_last_of(L"/");
    std::wstring path = szPullfilePath.substr(0, found + 1);
    std::wstring key = szPullfilePath.substr(found + 1);
    const TTexture* data = GetPtr(key);
    if (data != nullptr)
    {
        return data;
    }
    TTexture* newData = new TTexture;
    if (newData->Load(m_pDevice, szPullfilePath))
    {
        m_list.insert(std::make_pair(key, newData));
        return newData;
    }
    delete newData;
    return nullptr;
}
const TTexture* TTextureMgr::GetPtr(std::wstring key)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return nullptr;
    }
    return iter->second;
}
bool TTextureMgr::Get(std::wstring key, TTexture& ret)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return false;
    }
    ret = *(iter->second);
    return true;
}
bool TTextureMgr::Release()
{
    for (auto& data : m_list)
    {
        data.second->Release();
        delete data.second;
    }
    m_list.clear();  
    return true;
}
TTextureMgr::TTextureMgr()
{

}
TTextureMgr::~TTextureMgr()
{
    Release();
}