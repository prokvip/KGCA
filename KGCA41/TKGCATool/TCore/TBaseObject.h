#pragma once
#include "TDevice.h"
#include "TTextureManager.h"
#include "TShaderManager.h"
#include "TCollision.h"
struct SimpleVertex
{
	TVector3    p;	
	TVector4  c;
	TVector2  t;
	SimpleVertex() {}
	SimpleVertex(TVector3 vp, TVector4 vc, TVector2 vt)
	{
		p = vp;
		c = vc;
		t = vt;
	}
};
struct PNCT_VERTEX
{
	TVector3    p;
	TVector3    n;
	TVector4   c;
	TVector2   t;
	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3 vp, TVector3 vn, TVector4 vc, TVector2 vt)
	{
		p = vp;
		n = vn;
		c = vc;
		t = vt;
	}
};
struct VS_CONSTANT_BUFFER
{
	TMatrix  matWorld;
	TMatrix  matView;
	TMatrix  matProj;	
	TMatrix  matWorldInverse;
	TVector4 vLightDir;
	TVector4 vLightPos;
	TVector4 vEyeDir;
	TVector4 vEyePos;
	float    x;
	float    y;
	float    z;
	float    fTimer;
};

struct IW_VERTEX
{
	TVector4   i;
	TVector4   w;
	IW_VERTEX() {}
	IW_VERTEX(TVector4 vIndex, TVector4 vWeight)
	{	
		i = vIndex;
		w = vWeight;		
	}
};
struct VS_CONSTANT_BONE_BUFFER
{
	TMatrix  matBone[255];
};
namespace TDX
{
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, 
									 void* pDataAddress,
									 UINT iNumVertex, 
									 UINT iVertexSize);
	class TBaseObject
	{
	public:
		bool		m_IsNullable = true;
		TMatrix		m_matWorld;
		TMatrix		m_matView;
		TMatrix		m_matProj;
		DWORD		m_dwFace;
	public:
		ID3D11Device*			m_pd3dDevice = nullptr;// 디바이스 객체
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;
		ID3D11Buffer*			m_pVertexBuffer=nullptr;
		ID3D11Buffer*			m_pIndexBuffer = nullptr;
		VS_CONSTANT_BUFFER		m_cbData;
		ID3D11Buffer*			m_pConstantBuffer = nullptr;
		ID3D11InputLayout*		m_pVertexLayout = nullptr;
		TShader*				m_pShader = nullptr;
		TTexture*				m_pTexture = nullptr;
		std::wstring m_szShaderName;
		std::wstring m_szTextureName;
		ID3D11ShaderResourceView*		m_pTextureSRV = nullptr;
		std::vector<PNCT_VERTEX>		m_VertexList;
		std::vector<PNCT_VERTEX>		m_InitVertexList;
		std::vector<DWORD>				m_IndexList;
		TRect							m_rtCollision;
		T_BOX							m_tBox;
	public:
		ID3D11VertexShader*		m_pVS = nullptr;
		ID3D11PixelShader*		m_pPS = nullptr;
		ID3D11HullShader*		m_pHS = nullptr;
		ID3D11DomainShader*		m_pDS = nullptr;
		ID3D11GeometryShader*	m_pGS = nullptr;
		ID3D11ComputeShader*	m_pCS = nullptr;
		ID3DBlob*				m_pVSCode = nullptr;
		ID3DBlob*				m_pPSCode = nullptr;
	public:
		virtual bool		Create(	ID3D11Device* pd3dDevice,
									ID3D11DeviceContext* pImmediateContext,
									std::wstring shadername, std::wstring texturename);
		virtual void		CreateVertexData();
		virtual void		CreateIndexData();
		virtual void		CreateConstantData();
		virtual HRESULT		CreateVertexBuffer();
		virtual HRESULT		CreateIndexBuffer();
		virtual HRESULT		CreateConstantBuffer();
		virtual bool		CreateShader(std::wstring filename);
		virtual HRESULT     CreateVertexShader(std::wstring filename);
		virtual HRESULT     CreatePixelShader(std::wstring filename);
		virtual HRESULT     CreateVertexLayout();
		virtual void		UpdateVertexBuffer();
		virtual void		UpdateConstantBuffer();
		virtual bool		LoadTexture(W_STR texturename);
	public:
		virtual bool		Init();
		virtual bool		Frame();
		virtual bool		PreRender(ID3D11DeviceContext* pContext);
		virtual bool		Render(ID3D11DeviceContext* pContext);
		virtual bool		RenderShadow(ID3D11DeviceContext* pContext);
		virtual bool		PostRender(ID3D11DeviceContext* pContext);
		virtual bool		Release();
		virtual void		SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
	public:
		TBaseObject()
		{
			m_IsNullable = false;
		}
	};
};
using namespace TDX;
