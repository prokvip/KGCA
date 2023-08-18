#pragma once
#include "TCore.h"
#include "TPlaneObj.h"

class Sample : public TCore
{
    TObject*                m_pMapObj = nullptr;
    TTextureMgr             m_texMgr;
    TShaderMgr              m_shaderMgr;
    std::vector<TObject*>   m_NpcList;
    ID3D11BlendState*       m_AlphaBlend = nullptr;    
    TMatrix                 m_matView;
    TMatrix                 m_matOrthoProjection;
    TVector3                m_vCameraPos;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

