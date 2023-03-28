#pragma once
#include "TObject3D.h"


namespace TBasisFBX
{
	struct PNCT
	{
		TBasisFBX::TVector3 p;
		TBasisFBX::TVector3 n;
		TBasisFBX::TVector4 c;
		TBasisFBX::TVector2 t;
	};
	struct TVertexIW
	{
		float  i[4];
		float  w[4];
		TBasisFBX::TVector3 tan;
		TVertexIW()
		{
			i[0] = i[1] = i[2] = i[3] = 0;
			w[0] = w[1] = w[2] = w[3] = 0.0f;
		}
	};
	// ��Ƽ��Ʈ��  (VB[0], VB[1])
	// ��ǲ���̾ƿ� ( PNCTIW ) -> �������̴� ����

	struct TTrack
	{
		UINT	iFrame;
		TBasisFBX::TMatrix matTrack;
		// SRT
		TBasisFBX::TVector3	   s;
		TBasisFBX::TQuaternion r;
		TBasisFBX::TVector3    t;
	};
	struct TWeight
	{
		std::vector<int>	Index;  // ������ ��ġ�� ����� �ε���
		std::vector<float>  Weight; // ����ġ
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
	class TBASISFBX_API TFbxModel : public TObject3D
	{
	public:
		int		 m_iIndex = -1;
		bool	 m_bSkinned = false;
		TMatrix  m_matLocal;
		TMatrix  m_matAnim;
		FbxNode* m_pFbxParent = nullptr;
		FbxNode* m_pFbxNode = nullptr;
		TFbxModel* m_pParentObj = nullptr;
		std::wstring m_szTexFileName;
		// submaterial
		std::vector<std::wstring>  m_szTexFileList;
		using TSubVertex = std::vector<TVertex>;
		using TSubVertexIW = std::vector<TVertexIW>;
		std::vector<TSubVertex>      m_pSubVertexList;
		std::vector<TSubVertexIW>    m_pSubIWVertexList;
		std::vector<TWeight>		 m_WeightList;

		std::vector<ID3D11Buffer*>   m_pVBList;
		std::vector<ID3D11Buffer*>   m_pVBWeightList;
		std::vector<TTexture*>		 m_pTextureList;

		std::vector<TTrack>					m_AnimTrack;
		std::map<std::wstring, TMatrix>		m_dxMatrixBindPoseMap;

		TBoneWorld			m_matBoneArray;

	public:
		virtual bool    SetVertexData() override;
		virtual bool	CreateVertexBuffer()override;
		virtual bool    SetIndexData() override;
		virtual bool	CreateInputLayout() override;
		virtual bool	PostRender() override;
		virtual bool    Release() override;
		virtual void    GenAABB()override;
	};
	struct TScene
	{
		UINT   iStart;
		UINT   iEnd;
		UINT   iFrameSpeed;
	};
	class TFbxImporter : public TObject3D
	{
	public:
		TScene		m_Scene;
		float m_fDir = 1.0f;
		float m_fTime = 0.0f;
		float m_fSpeed = 1.0f;
		TBoneWorld	  m_matBoneArray;
	public:
		FbxManager* m_pFbxManager;
		FbxImporter* m_pFbxImporter;
		FbxScene* m_pFbxScene;
		FbxNode* m_pRootNode;

		std::map<FbxNode*, int>				m_pFbxNodeMap;
		std::map<std::wstring, TFbxModel*>	m_pFbxModelMap;

		std::vector<TFbxModel*>  m_DrawList;
		std::vector<TFbxModel*>  m_TreeList;
		ID3D11Buffer* m_pBoneCB = nullptr;
	public:
		bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
		virtual bool	Load(std::string filename);
		virtual void    PreProcess(FbxNode* node, TFbxModel* fbxParent = nullptr);
		virtual void	ParseMesh(TFbxModel* pObject);
		std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);

		void ReadTextureCoord(FbxMesh* pFbxMesh,
			FbxLayerElementUV* pUVSet,
			int vertexIndex,
			int uvIndex,
			FbxVector2& uv);

		FbxColor ReadColor(const FbxMesh* mesh,
			DWORD dwVertexColorCount,
			FbxLayerElementVertexColor* pVertexColorSet,
			DWORD dwDCCIndex, DWORD dwVertexIndex);
		FbxVector4 ReadNormal(const FbxMesh* mesh,
			int controlPointIndex,
			int vertexCounter);
		FbxVector4 ReadNormal(const FbxMesh* mesh,
			DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
			int controlPointIndex, int iVertexIndex);
		FbxVector4 ReadTangent(const FbxMesh* mesh,
			DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
			DWORD dwDCCIndex, DWORD dwVertexIndex);
		int GetSubMaterialIndex(int iPlygon, FbxLayerElementMaterial* pMtrl);
		bool	ParseMeshSkinning(FbxMesh* pFbxMesh, TFbxModel* pObject);
	public:
		TMatrix     DxConvertMatrix(TMatrix m);
		TMatrix     ConvertMatrix(FbxMatrix& m);
		TMatrix     ConvertAMatrix(FbxAMatrix& m);
		void		ParseAnimation();
	public:
		virtual bool	Init();
		virtual bool	Frame();
		virtual bool	Render();
		virtual bool	Release();
		virtual bool	CreateConstantBuffer(ID3D11Device* pDevice);
	};

};