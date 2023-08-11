#pragma once
#include "TCore.h"
#include "TObject.h"
class Sample : public TCore
{
    std::vector<TObject*>   m_ObjList;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

