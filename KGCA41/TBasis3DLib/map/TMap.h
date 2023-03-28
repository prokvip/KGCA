#pragma once
#include "../TShape.h"
using namespace DX;

#define RANDOM(n)			(rand() % (n))
#define RANDOMSCOPE(s,e)	(rand() %(e-s))+s;
#define RANDOMIZE()			srand((unsigned)time(NULL))

struct TMapDesc
{
	int				iNumCols;
	int				iNumRows;
	float			fCellDistance;	
	float			fScaleHeight;
	T_STR			strTextureFile;
	T_STR			strShaderFile;	
};

class TMap : public TActor
{
public:
	TMapDesc				m_MapDesc;
	vector<PNCT_VERTEX>		m_VertexList;
	DWORD					m_dwShaderIndex;
	int						m_iNumVertices;
	int						m_iNumFace;
	int						m_iDiffuseTex;
	int						m_iNumCols;
	int						m_iNumRows;
	int						m_iNumCellCols;
	int						m_iNumCellRows;
	float					m_fCellDistance;
	TMatrix				m_matWorld;
	TMatrix				m_matView;
	TMatrix				m_matProj;
	TMatrix				m_matNormal;
	TVector3				m_vLightDir;
	int*					m_pNormalLookupTable;
	TVector3*			m_pFaceNormals;
	bool					m_bStaticLight;
public:
	bool		Frame();
	bool		Render(ID3D11DeviceContext*  pContext);
	bool		Release();
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateIndexBuffer();
	bool		CreateVertexData();
	bool		CreateIndexData();
	bool		UpdateBuffer();
	HRESULT		SetInputLayout();
public:
	DXGI_FORMAT			MAKE_TYPELESS(DXGI_FORMAT format);
	DXGI_FORMAT			MAKE_SRGB(DXGI_FORMAT format);
public:	
	virtual bool		GenVertexNormal();
	virtual bool		ReLoadVBuffer();
	virtual bool		ReLoadIBuffer();
	virtual void		UpdateIndexBuffer(ID3D11DeviceContext*  pContext, DWORD* pdwIndexArray, int iFaceCount);
	virtual void		CalcVertexColor(TVector3 vLightDir);
	virtual bool		CreateMap(TMapDesc& MapDesc);
	virtual bool		Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext*  pImmediateContext);
	virtual bool		Load(TMapDesc& MapDesc);
	//--------------------------------------------------------------------------------------
	// 정점 노말 연산를 위한 초기화 함수 및 페이스 노말 계산
	//--------------------------------------------------------------------------------------	
	virtual void		InitFaceNormals();
	virtual void		CalcFaceNormals();
	virtual TVector3	ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	//--------------------------------------------------------------------------------------
	// 정점 노말 룩업 테이블 생성 및 계산
	//--------------------------------------------------------------------------------------	
	virtual void		GenNormalLookupTable();
	virtual void		CalcPerVertexNormalsFastLookup();	
	virtual float		GetHeightmap(int row, int col);	
	virtual float		GetHeightOfVertex(UINT Index);
	virtual TVector3 GetNormalOfVertex(UINT Index);
	virtual TVector4 GetColorOfVertex(UINT Index);
	virtual TVector2 GetTextureOfVertex(float fOffsetX, float fOffsetY);
	virtual float		GetHeight(float fPosX, float fPosZ);
	virtual float		Lerp(float fStart, float fEnd, float fTangent);
public:
	TMap(void);
	virtual ~TMap(void);
};
