#pragma once
#include "TObjStd.h"
class TMesh;
class TActor;

struct TVertexLayout
{
	vector<D3D11_INPUT_ELEMENT_DESC> Element;
};
struct TObjWM
{
	TActor*				m_pModel;
	float				m_fElapseTime;
	TScene				m_Scene;
	vector<TMatrix>	m_matCalculation;
	vector<TMatrix>	m_matAnim;
	vector<int>			m_iParent;
	TObjWM(int iNumMesh)
	{
		m_pModel = NULL;
		m_fElapseTime = 0.0f;
		m_matCalculation.resize(iNumMesh);
		m_matAnim.resize(iNumMesh);
		m_iParent.resize(iNumMesh);
		for (int iMesh = 0; iMesh < iNumMesh; iMesh++)
		{
			D3DXMatrixIdentity(&m_matCalculation[iMesh]);
			D3DXMatrixIdentity(&m_matAnim[iMesh]);
			m_iParent[iMesh] = -1;
		}
	}
};

class TActor //: public TExpMesh
{
public:	
	//--------------------------------------------------------------------------------------
	// Collision Data
	//--------------------------------------------------------------------------------------
	T_BOX                       m_Box;
	T_BOX						m_InitBox;
	T_SPHERE                    m_Sphere;
	T_PLANE                     m_Plane;
	float						m_fTickSpeed;
public:
	TVector3 m_vCenter;
	TVector3 m_vPrevCenter;
	TVector3 m_vMove;
	TVector3 m_vLook; // Z
	TVector3 m_vUp; // Y
	TVector3 m_vSide; // X
	bool m_bUpdateCollision;
public:
	vector<shared_ptr<TMesh>>	m_pMesh;
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pContext;
	DX::TDxObject				m_dxobj;
	std::vector< T_STR >     m_InfluenceNames;
	std::unordered_map<T_STR, TMatrix> m_matBindPoseMap;
	void AddInfluence(T_STR InfluenceName, TMatrix& mat)
	{
		m_InfluenceNames.push_back(InfluenceName);
		m_matBindPoseMap[InfluenceName] = mat;
		//m_VertexFormat.m_bSkinData = true;
	}
	VS_CONSTANT_BUFFER			m_cbData;
	
	TMatrix					m_matControlWorld;
	TMatrix					m_matWorld;
	TMatrix					m_matView;
	TMatrix					m_matProj;
	vector<DWORD>				m_IndexList;
	vector<PNCT_VERTEX>			m_VertexList;
	TScene				m_Scene;
	FILETYPE			m_ObjType;
	float				m_fElapseTime;
	int					m_iStartFrame;
	int					m_iLastFrame;
	T_STR				m_szDirName;
	T_STR				m_szName;
	///  Buffer Comine 
	UINT				m_iMaxVertex;
	UINT				m_iMaxIndex;
	UINT				m_iBeginPosVB;
	UINT				m_iBeginPosIB;
	bool				m_bOptimization;
	int					m_iMatrixIndex;	

public:
	float				SetAnimationSpeed(float fSpeed)
	{
		m_fTickSpeed = fSpeed;
		return m_fTickSpeed;
	}
	
public:
	ID3D11Buffer*				GetVB();
	ID3D11Buffer*				GetIB();
	virtual int					GetMeshCounter() { return 0; }
	virtual void				SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	virtual void				SetAmbientColor(float fR, float fG, float fB, float fA);
	virtual bool				Convert(ID3D11Device* pDevice);
	virtual bool				Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	virtual HRESULT				LoadShaderFile(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile);
	virtual	void				ResetShader();
	virtual HRESULT				SetInputLayout();
	virtual bool				CreateVertexData();
	virtual bool				CreateIndexData();
	virtual HRESULT				CreateVertexBuffer();
	virtual HRESULT				CreateIndexBuffer();
	virtual HRESULT				CreateConstantBuffer();
	virtual HRESULT				LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString);
	virtual HRESULT				CreateResource();
	virtual HRESULT				DeleteResource();
	virtual bool				UpdateBuffer();
	virtual void				ObjectFrame(TObjFrame* pObjFrame, float fTickFrame) {};
	virtual void				ObjectRender(ID3D11DeviceContext*    pContext, TObjFrame* pObjFrame) {};
	virtual bool				Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString = 0);
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	virtual bool				Render(ID3D11DeviceContext*		pContext);
	// 지연 컨텍스트의 사용시에 즉시 컨텍스트로 변경하기 위해서 로직에서 가상 함수화 하였다.
	virtual void				UpdateConstantBuffer(ID3D11DeviceContext* pContext, TActor* pParent= NULL);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 dwColor);
	virtual bool				Release();
	virtual void				SetMatrixIndex(int iMatrixIndex) { m_iMatrixIndex = iMatrixIndex; }
	// 기본적인 바운딩박스 및 스피어를 계산해 둔다.
	virtual void				SetCollisionData(TMatrix& matWorld);
public:
	TActor(void);
	virtual ~TActor(void);
};