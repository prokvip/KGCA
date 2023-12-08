#pragma once
#include "TObject.h"
struct TBoneWorld
{
	TMatrix  matBoneWorld[255];
};

struct TVertexIW
{
	float  i[4];
	float  w[4];
	TVector3 tan;
	TVertexIW()
	{
		i[0] = i[1] = i[2] = i[3] = 0;
		w[0] = w[1] = w[2] = w[3] = 0.0f;
	}
};
struct TWeight
{
	std::vector<int>	Index;  // 영향을 미치는 행렬의 인덱스
	std::vector<float>  Weight; // 가중치
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > Weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					Weight[j] = Weight[j - 1];
				}
				Index[i] = iBoneIndex;
				Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	TWeight()
	{
		Index.resize(8);
		Weight.resize(8);
	}
};
struct TFbxMesh
{
	C_STR      m_csName;
	TMatrix    m_matWorld;
	TFbxMesh*  m_pParent = nullptr;

	BOOL	   m_bSkinning=FALSE;
	UINT       m_iBoneIndex;

	UINT  m_iNumPolygon = 0;
	using SUBMATERIAL = std::vector<PNCT_Vertex>;
	using TSubVertexIW = std::vector<TVertexIW>;

	std::vector<SUBMATERIAL> 		m_TriangleList;
	std::vector<TSubVertexIW>		m_pSubIWVertexList;
	std::vector<TWeight>			m_WeightList;

	std::vector<UINT> 				m_TriangleOffsetList;
	std::vector<W_STR>				m_szTextureFileName;
	std::vector<TMatrix>		    m_MatrixArray;
	std::vector<TFbxMesh*>		    m_pChild;

	std::map<std::wstring, TMatrix>		m_dxMatrixBindPoseMap;

};
class TFbxObj : public TObject
{		
public:
	enum { BONE_NODE = 0, MESH_NODE, SKIN_NODE, };
	std::vector<TVertexIW>		m_pVertexListIW;
	ID3D11Buffer*				m_pVBWeightList;
public:
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;
	BOOL    m_iBone;
	
	TFbxObj*	m_pParent = nullptr;
	BOOL	   m_bSkinning = FALSE;
	UINT       m_iBoneIndex;

	UINT		m_iNumPolygon = 0;
	using SUBMATERIAL = std::vector<PNCT_Vertex>;
	using TSubVertexIW = std::vector<TVertexIW>;


	std::vector<SUBMATERIAL> 		m_TriangleList;
	std::vector<TSubVertexIW>		m_pSubIWVertexList;
	std::vector<TWeight>			m_WeightList;

	std::vector<UINT> 				m_TriangleOffsetList;
	std::vector<W_STR>				m_szTextureFileName;
	std::vector<std::vector<TMatrix>>		    m_MatrixArray;
	std::vector<TFbxObj*>						m_pChild;
	std::map<std::wstring, TMatrix>				m_dxMatrixBindPoseMap;
	std::map<std::wstring, int>					m_pFbxNodeMap;
	std::vector<const TTexture*>				m_TexArray;
public:
	std::vector<std::wstring>				m_pFbxModeNameList;
	std::vector<std::shared_ptr<TFbxObj>>	m_TreeList;
	std::vector<std::shared_ptr<TFbxObj>>	m_DrawList;
	
	
	TBoneWorld								m_matBoneArray;
	ID3D11Buffer*							m_pBoneCB = nullptr;
	void	SetAnim(UINT startFrame, UINT endFrame, UINT frameSpeed = 30, UINT tickForFrame = 160);
	UINT    GetStartFrame();
	UINT    GetEndFrame();
	UINT    GetFrameSpeed();
	UINT    GetTickForFrame();
public:	
	bool	CreateConstantBuffer() override;
	bool	CreateInputLayout() override;
	bool	CreateVertexBuffer() override;
	void    LoadTextureArray(W_STR filename);
};


