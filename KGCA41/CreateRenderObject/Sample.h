#pragma once
#include "TCore.h"
#include "TPlaneObj.h"
#include "TPlayer.h"
class Sample : public TCore
{
    TObject*                m_pMapObj = nullptr;
    TObject*                m_pPlayer = nullptr;
    TTextureMgr             m_texMgr;
    TShaderMgr              m_shaderMgr;
    std::vector<TObject*>   m_NpcList;
    ID3D11BlendState*       m_AlphaBlend = nullptr;   
    
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

