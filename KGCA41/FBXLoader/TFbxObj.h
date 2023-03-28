#pragma once
#include "TFbxAnimation.h"
#include "TFbxMaterial.h"
#include "TMesh.h"

struct AnimationScanNode
{
	INT iParentIndex;
	FbxNode* pNode;
	ExportAnimationTrack* pTrack;
	DWORD dwFlags;
	XMFLOAT4X4 matGlobal;
};

typedef TData<PNCT3_VERTEX>						tFbxData;
typedef std::vector<shared_ptr<tFbxData>>		tFbxMeshData;
typedef std::vector<TFbxMaterial*>				fbxMtrlList;
typedef std::vector<fbxMtrlList>				fbxMaterialList;
typedef std::unordered_map<T_STR, TMatrix>	DxMatrixMap;
typedef std::unordered_map<FbxNode*, FbxMatrix> PoseMap;
typedef std::unordered_map<FbxNode*, TMesh*>	tMeshMap;
typedef std::vector<AnimationScanNode>			ScanList;

struct TLayer
{
	FbxLayer* pLayer;
	std::vector<FbxLayerElementUV*>				VertexUVSets;
	std::vector<FbxLayerElementMaterial*>		VertexMaterialSets;
	std::vector<FbxLayerElementVertexColor*>	VertexColorSets;
	std::vector<FbxLayerElementNormal*>			VertexNormalSets;
	std::vector<FbxLayerElementTangent*>		VertexTangentSets;
};
struct TLayerInfo
{
	FbxMesh*				pFbxMesh;
	std::vector<TLayer>		tLayerList;	
};
class TFbxObj : public TActor
{
private:
	static FbxManager*			g_pSDKManager;
	FbxImporter*				m_pFbxImporter;
	FbxScene*					m_pFBXScene;
	FbxAxisSystem				m_SceneAxisSystem;
	FbxSystemUnit				m_SceneSystemUnit;
public:
	TFbxObj*					m_pSkeletonMesh;
	TMatrix*					m_pMatrix;
	ID3D11Buffer*               m_pBoneBuffer;
	ID3D11ShaderResourceView*	m_pBoneBufferRV;
	tFbxMeshData				m_pData;
public:	
	std::vector<FbxPose*>		m_BindPoses;
	PoseMap						m_BindPoseMap;
	vector<ExportAnimation*>    m_pNodeAnimList;	
	fbxMaterialList				m_pMaterialList;
	DxMatrixMap					m_matFrameMatrixMap;
	tMeshMap					m_tMeshMap;	
	bool						m_bBindPoseFixupRequired;
	ScanList					m_Scanlist;
	TVector3					m_vLight = TVector3(0, 0, 1);
	std::vector<TLayerInfo>		m_pFbxLayerList;
public:
	std::vector<FbxSurfaceMaterial*>	m_pRootMtrlList;
//public:
//	ComPtr<ID3D11Buffer>                m_pBoneFrameBuffer;
//	ComPtr<ID3D11ShaderResourceView>	m_pBoneFrameBufferRV;
//	using animFrame = std::vector<TMatrix>;
//	std::vector<animFrame>				m_pBoneFrameMatrix;
//	bool SetBoneFrameBuffer(ID3D11Device* pd3dDevice);
//	void SetBoneFrameMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh);
public:	
	void SetSkeletonMesh(TFbxObj* pSkeleton);
	void SetBindPose();
	void PreProcess(FbxNode* pNode);
	HRESULT Initialize(const char* pFileName);
	TFbxMaterial* ParseMaterial(FbxSurfaceMaterial* pFbxMaterial);
	VOID ClearScene();
	VOID ParseNode(FbxNode* pNode, FbxScene* pScene, TMesh* pParent, const TMatrix& matParentWorld );
	VOID ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh,TMesh* pMesh, tFbxData* pData,	BOOL bSubDProcess = FALSE, const CHAR* strSuffix = NULL);
	TMatrix ParseTransform(FbxNode* pNode, const TMatrix& matParentWorld, const BOOL bWarnings = TRUE);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
	bool ParseMeshSkinning(FbxMesh* pMesh, SkinData* pSkinData);	
	TFbxMaterial* ParseMaterialInLayer(FbxMesh* pMesh,FbxLayer* pLayer,	DWORD dwMaterialIndex);
	BOOL ExtractTextures(FbxProperty Property, const CHAR* strParameterName, TFbxMaterial* pMaterial, DWORD dwFlags);
	VOID AddTextureParameter(TFbxMaterial* pMaterial, const CHAR* strParamName, DWORD dwIndex, const CHAR* strFileName, DWORD dwFlags);
	void CaptureBindPoseMatrix( SkinData* pSkinData, FbxNode* pNode, const FbxMatrix& matBindPose);
	void ParseAnimation(FbxScene* pFbxScene);
	void ParseAnimStack(FbxScene* pFbxScene, FbxString* strAnimStackName);	
	void AddAnimation(ExportAnimation* pAnim);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(	const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets, 
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(  const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
							DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
							DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets, 
							DWORD dwDCCIndex, DWORD dwVertexIndex);
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh, TFbxObj* pAnimMesh);
public:
	bool		LoadFBX(const char* pFileName);
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, TMesh* pMesh);
	bool		SetBuffer(ID3D11Device* pd3dDevice);
	bool		Load(ID3D11Device* pd3dDevice, const char* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	HRESULT		SetInputLayout();
	HRESULT		LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, TActor* pParent);
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Frame();
	bool		Convert(ID3D11Device* pDevice);
	bool		GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>> pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack);
	TMatrix	Interpolate(TMesh* pMesh, TMatrix* matParents, float fFrameTick);
public:
	void AddKey(AnimationScanNode& asn, const AnimationScanNode* pParent, FbxAMatrix& matFBXGlobal, float fTime);
	void ParseNodeA(FbxNode* pNode, ScanList& scanlist, DWORD dwFlags, INT iParentIndex, bool bIncludeNode);
	void CaptureAnimation(ScanList& scanlist, ExportAnimation* pAnim, FbxScene* pFbxScene);
	void TransformMatrix(XMFLOAT4X4* pDestMatrix, const XMFLOAT4X4* pSrcMatrix);
public:
	TFbxObj();
	TFbxObj(TFbxObj& obj);
	TFbxObj(TFbxObj&& obj);
	virtual ~TFbxObj();
};

