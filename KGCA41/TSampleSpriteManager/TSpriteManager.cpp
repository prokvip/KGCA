#include "TSpriteManager.h"
bool TSpriteManager::GameDataLoad(const TCHAR* pszLoad)
{
    TCHAR pBuffer[256] = { 0 };
    TCHAR pTemp[256] = { 0 };

    int iNumSprite = 0;
    FILE* fp_src;
    _wfopen_s(&fp_src, pszLoad, _T("rt"));
    if (fp_src == NULL) return false;

    _fgetts(pBuffer, _countof(pBuffer), fp_src);
    _stscanf_s(pBuffer, _T("%s%d%s"), pTemp, (unsigned int)_countof(pTemp), &iNumSprite);
    m_rtSpriteList.resize(iNumSprite);
   

    for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
    {
        int iNumFrame = 0;
        _fgetts(pBuffer, _countof(pBuffer), fp_src);
        _stscanf_s(pBuffer, _T("%s %d"), pTemp, (unsigned int)_countof(pTemp), &iNumFrame);
        //m_rtSpriteList[iCnt].resize(iNumFrame);
        m_rtNameList.push_back(pTemp);

        RECT rt;
        for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
        {
            _fgetts(pBuffer, _countof(pBuffer), fp_src);
            _stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
                &rt.left, &rt.top, &rt.right, &rt.bottom);
            m_rtSpriteList[iCnt].push_back(rt);
        }
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

    if (GameDataLoad(filename.c_str()) == false)
    {
        return false;
    }
    filelist.push_back(filename);

    HRESULT hr;
    for (int iSp = 0; iSp < m_rtSpriteList.size(); iSp++)
    {
        // 중복제거
        auto data = Find(m_rtNameList[iSp]);
        if (data != nullptr) continue;

        TSprite* pNewData = new TSprite;
        pNewData->m_szName = m_rtNameList[iSp];
        pNewData->m_uvArray = m_rtSpriteList[iSp];

        if (pNewData)
        {
            hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, 
                                    filename);
            if (SUCCEEDED(hr))
            {
                m_List.insert(std::make_pair(pNewData->m_szName, pNewData));
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
        TSprite* pData = iter->second;
        return *pData;
    }  
}
TSprite* TSpriteManager::GetPtr(W_STR name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        TSprite* pData = iter->second;
        return pData;
    }
    return nullptr;
}
TSprite* TSpriteManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    return nullptr;
}
TSpriteManager::TSpriteManager()
{
}
bool TSpriteManager::Release()
{
    for (auto data : m_List)
    {
        TSprite* pData = data.second;
        if (pData) pData->Release();
        delete pData;
    }
    m_List.clear();
    return true;
}
TSpriteManager::~TSpriteManager()
{
    Release();
}