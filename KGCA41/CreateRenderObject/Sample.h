#pragma once
#include "TCore.h"
#include "TNpcObj.h"
#include "TPlayer.h"
class Sample : public TCore
{
    TObject*                m_pMapObj = nullptr;
    TObject*                m_pPlayer = nullptr;   
    std::vector<TObject*>   m_NpcList;
    ID3D11BlendState*       m_AlphaBlend = nullptr;   
    
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
public:
    void  CreateBlendState();
};

