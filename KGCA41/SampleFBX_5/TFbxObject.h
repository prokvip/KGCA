#pragma once
#include "TObject.h"
struct TFbxMesh
{
	C_STR      m_csName;
	TMatrix    m_matWorld;
	TFbxMesh* m_pParent = nullptr;

	UINT  m_iNumPolygon = 0;
	using SUBMATERIAL = std::vector<PNCT_Vertex>;
	std::vector<SUBMATERIAL> 		m_TriangleList;
	std::vector<UINT> 				m_TriangleOffsetList;
	std::vector<W_STR>				m_szTextureFileName;
	std::vector<TMatrix>		    m_MatrixArray;
	std::vector<TFbxMesh*>		    m_pChild;
};
class TFbxObj : public TObject
{	
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;
public:
	std::vector<std::shared_ptr<TFbxMesh>>		    m_tMeshList;

	void	SetAnim(UINT startFrame, UINT endFrame, UINT frameSpeed = 30, UINT tickForFrame = 160);
	UINT    GetStartFrame();
	UINT    GetEndFrame();
	UINT    GetFrameSpeed();
	UINT    GetTickForFrame();
};


