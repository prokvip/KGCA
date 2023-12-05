#pragma once
#include "TObject.h"
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
	TFbxMesh* m_pParent = nullptr;

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

};
class TFbxObj : public TObject
{	
	UINT	m_iStartFrame = 0;
	UINT	m_iEndFrame = 0;
	UINT	m_iFrameSpeed = 30;
	UINT	m_iTickForFrame = 160;
public:
	std::vector<std::shared_ptr<TFbxMesh>>		    m_tMeshList;
	std::map<std::wstring, TMatrix>		m_dxMatrixBindPoseMap;
	

	void	SetAnim(UINT startFrame, UINT endFrame, UINT frameSpeed = 30, UINT tickForFrame = 160);
	UINT    GetStartFrame();
	UINT    GetEndFrame();
	UINT    GetFrameSpeed();
	UINT    GetTickForFrame();
//public:
//	bool	CreateInputLayout() override;
//	bool	CreateVertexBuffer() override;
//	std::vector<ID3D11Buffer*>   m_pVBWeightList;

	
};


