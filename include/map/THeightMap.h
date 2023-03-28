#pragma once
#include "TMap.h"

class THeightMap : public TMap
{
public:
	vector<float>		m_fHeightList;
public:
	bool		CreateHeightMap(const TCHAR* strHeightMap);
	float		GetHeightOfVertex(UINT Index);
	TVector4 GetColorOfVertex(UINT Index);	
	float		GetHeightmap(int row, int col);
public:
	THeightMap(void);
	virtual ~THeightMap(void);
};
