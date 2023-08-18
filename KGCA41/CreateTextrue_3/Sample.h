#pragma once
#include "TCore.h"
#include "TPlaneObj.h"

class Sample : public TCore
{
    TTextureMgr             m_texMgr;
    TShaderMgr              m_shaderMgr;
    std::vector<TObject*>   m_ObjList;
    ID3D11BlendState*       m_AlphaBlend = nullptr;    
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

