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
};
class TMap :public TModel
{
public: 
    TMapInfo m_info;
public:
    bool    Load(TMapInfo& info);
	bool	CreateVertexData() override;
    bool	CreateIndexData() override;
   
};

