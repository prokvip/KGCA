#include "TSpriteManager.h"
TSprite  TSpriteManager::m_NullSprite;
bool TSpriteManager::GameDataLoad(const TCHAR* pszLoad)
{
    TCHAR pBuffer[256] = { 0 };
    TCHAR pTemp[256] = { 0 };
    TCHAR pTexturePath[256] = { 0 };
    TCHAR pMaskTexturePath[256] = { 0 };
    TCHAR pShaderPath[256] = { 0 };

    int iNumSprite = 0;
    FILE* fp_src;
    _wfopen_s(&fp_src, pszLoad, _T("rt"));
    if (fp_src == NULL) return false;

    _fgetts(pBuffer, _countof(pBuffer), fp_src);
    _stscanf_s(pBuffer, _T("%s"), pTemp, (unsigned int)_countof(pTemp));
    //m_rtSpriteList.resize(iNumSprite);
   

    for (;;)
    {
        int iNumFrame = 0;
        _fgetts(pBuffer, _countof(pBuffer), fp_src);
        _stscanf_s(pBuffer, _T("%s %d %s%s%s"), 
            pTemp, (unsigned int)_countof(pTemp), &iNumFrame,
            pTexturePath, (unsigned int)_countof(pTexturePath),
            pMaskTexturePath, (unsigned int)_countof(pMaskTexturePath),
            pShaderPath, (unsigned int)_countof(pShaderPath));
        
        W_STR name = pTemp;
        if (name == L"#END")
        {
            break;
        }

        m_rtNameList.push_back(pTemp);
        m_TextureNameList.push_back(pTexturePath);
        m_MaskTextureNameList.push_back(pMaskTexturePath);
        m_ShaderNameList.push_back(pShaderPath);

        RECT_ARRAY rtList;
        RECT rt;
        for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
        {
            _fgetts(pBuffer, _countof(pBuffer), fp_src);
            _stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
                &rt.left, &rt.top, &rt.right, &rt.bottom);
            rtList.push_back(rt);
        }
        m_rtSpriteList.push_back(rtList);
    }
    fclose(fp_src);


    return true;
}
void TSpriteManager::SetDevice(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}
bool TSpriteManager::Load(std::wstring filename)
{
    m_rtSpriteList.clear();
    m_rtNameList.clear();
    m_TextureNameList.clear();
    m_MaskTextureNameList.clear();
    m_ShaderNameList.clear();

    if (GameDataLoad(filename.c_str()) == false)
    {
        return false;
    }

    for (int ifile = 0; ifile < filelist.size(); ifile++)
    {
        if (filelist[ifile] == filename)
        {
            return true;
        }
    }
    filelist.push_back(filename);

    HRESULT hr;
    for (int iSp = 0; iSp < m_rtSpriteList.size(); iSp++)
    {
        // 중복제거
        auto data = Find(m_rtNameList[iSp]);
        if (data != nullptr) continue;

        std::unique_ptr<TSprite> pNewData = std::make_unique<TSprite>();
        pNewData->m_szName = m_rtNameList[iSp];
        pNewData->m_szTexturePath = m_TextureNameList[iSp];
        pNewData->m_szMaskTexturePath = m_MaskTextureNameList[iSp];
        pNewData->m_szShaderPath = m_ShaderNameList[iSp];
        pNewData->m_uvArray = m_rtSpriteList[iSp];

        if (pNewData)
        {
            bool bRet = pNewData->Load(m_pd3dDevice, m_pImmediateContext,filename);
            if (bRet)
            {
                m_List.insert(std::make_pair(pNewData->m_szName, std::move(pNewData)));
            }
            else
            {
                pNewData->Release();
                pNewData.reset();
            }
        }
    }    
    return true;
}
TSprite& TSpriteManager::Get(W_STR name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        TSprite* pData = iter->second.get();
        return *pData;
    }  
    return m_NullSprite;
}
TSprite* TSpriteManager::GetPtr(W_STR name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        TSprite* pData = iter->second.get();
        return pData;
    }
    return nullptr;
}
TSprite* TSpriteManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second.get();
    }
    return nullptr;
}
TSpriteManager::TSpriteManager()
{
}
bool TSpriteManager::Release()
{
    for (const auto& data : m_List)
    {   
        data.second.get()->Release();        
    }
    m_List.clear();
    return true;
}
TSpriteManager::~TSpriteManager()
{
    Release();
}