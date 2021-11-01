#pragma once
#include "TModel.h"
struct TMapInfo
{
    int m_iNumRow; // 2^n+1
    int m_iNumCol;
    int m_iNumRowCell;
    int m_iNumColCell;
    int m_iNumVertex;
    float m_fCellDistance;
    std::wstring szDefaultTexture;
};
class TMap :public TModel
{
public: 
    TMapInfo m_info;
public:
    bool    Load(TMapInfo& info, std::wstring vs, std::wstring ps);
	bool	CreateVertexData() override;
    bool	CreateIndexData() override;
   
};

