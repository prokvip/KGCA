#pragma once
#include "TShaderMgr.h"
#include "TTextureMgr.h"

namespace TBasisFBX
{
	struct SimpleVertex
	{
		TVector2 v;
		TVector2 t;
	};
	struct TVertex
	{
		TBasisFBX::TVector3 p; // ��ġ
		TBasisFBX::TVector3 n; // �븻
		TBasisFBX::TVector4 c; // �÷�
		TBasisFBX::TVector2 t; // �ؼ�
		TVertex(TBasisFBX::TVector3 p1,
			TBasisFBX::TVector3 n1,
			TBasisFBX::TVector4 c1,
			TBasisFBX::TVector2 t1)
		{
			p = p1;
			n = n1;
			c = c1;
			t = t1;
		}
		TVertex()
		{

		}
	};

	enum TCollisionType
	{
		Block = 0,
		Overlap,
		Ignore,
	};
	enum TSelectType
	{
		Select_Block = 0,
		Select_Overlap,
		Select_Ignore,
	};
	struct TIndex
	{
		DWORD _0;
		DWORD _1;
		DWORD _2;
	};

	struct TConstantData
	{
		TMatrix  matWorld;
		TMatrix  matView;
		TMatrix  matProj;
		TMatrix  matNormal;
		TVector4 Color;
		TVector4 Timer;
	};
	struct TBoneWorld
	{
		TMatrix  matBoneWorld[255];
	};
	struct TLightData
	{
		TVector4   vLightDir;
		TVector4   vLightPos;
		TVector4   vCameraDir;
		TVector4   vCameraPos;
		TMatrix	  matLight;// w*v*p*t
	};
	// 0001  - �⺻
	// 0010  - ����
	// 0100  - ����
	// 1000  - ����ź

	// 0111 = 7(�Ӽ�)
	// 1111 = 15
	enum TSelectState
	{
		T_DEFAULT = 0,  // Ŀ���� ���� ���� ��(T_FOCUS���¿��� �ٸ� ���� T_ACTIVE�ϸ� ��ȯ�ȴ�.)
		T_HOVER = 1,	// Ŀ���� ���� ���� ��
		T_FOCUS = 2,	// T_ACTIVE���¿��� ���� ��ư�� �ٸ� ������ ������ ��(���)
		T_ACTIVE = 4,	// ���콺 ���� ��ư ������ ���� ��
		T_SELECTED = 8, // T_ACTIVE ���¿��� ���ɹ�ư ������ ��
	};
	class TBASISFBX_API TBaseObject
	{
	public:
		std::wstring   m_csName;
	public:
		TBaseObject* m_pParent = nullptr;
		bool		m_bDead;
		int			m_iCollisionID;
		int			m_iSelectID;
		float		m_fSpeed;
		float		m_fWidth;
		float		m_fHeight;
		TBox		m_BoxCollision;
		DWORD		m_dwCollisonType;
		DWORD		m_dwSelectType;
		DWORD		m_dwSelectState;
		DWORD		m_dwPreSelectState;
		bool		m_bSelect;
		bool		m_bAlphaBlend;
	public:
		virtual void	HitOverlap(TBaseObject* pObj, DWORD dwState);
		virtual void	HitSelect(TBaseObject* pObj, DWORD dwState);
		virtual void	SetCollisionType(DWORD dwCollisionType, DWORD dwSelectType)
		{
			m_dwCollisonType = dwCollisionType;
			m_dwSelectType = dwSelectType;
		}
	public:

		TBaseObject()
		{
			m_bDead = false;
			m_bSelect = false;
			m_bAlphaBlend = true;
			m_dwSelectState = T_DEFAULT;
			m_iCollisionID = -1;
			m_iSelectID = -1;
			m_dwCollisonType = TCollisionType::Ignore;
			m_dwSelectType = TSelectType::Select_Ignore;
		}
		virtual ~TBaseObject() {}
	};

	class TBASISFBX_API TDxObject : public TBaseObject
	{
	public:
		TTexture* m_pColorTex = nullptr;
		TTexture* m_pMaskTex = nullptr;
		TShader* m_pVShader = nullptr;
		TShader* m_pPShader = nullptr;
		D3D11_TEXTURE2D_DESC		m_TextureDesc;
	public:
		std::vector<SimpleVertex> m_InitScreenList;
		std::vector<TVertex> m_VertexList;

		ID3D11Buffer* m_pVertexBuffer = nullptr;

		std::vector<DWORD> m_IndexList;
		ID3D11Buffer* m_pIndexBuffer = nullptr;

		TConstantData		m_ConstantList;
		ID3D11Buffer* m_pConstantBuffer = nullptr;
		TLightData			m_LightConstantList;
		ID3D11Buffer* m_pLightConstantBuffer = nullptr;

		ID3D11InputLayout* m_pVertexLayout = nullptr;
		ID3D11Device* m_pd3dDevice = nullptr;
		ID3D11DeviceContext* m_pContext = nullptr;
	public:
		void    SetDevice(ID3D11Device* m_pd3dDevice,
			ID3D11DeviceContext* m_pContext);
		virtual bool    LoadTexture(const TCHAR* szColorFileName,
			const TCHAR* szMaskFileName);
		virtual bool    SetVertexData();
		virtual bool    SetIndexData();
		virtual bool    SetConstantData();
		virtual bool	Create(ID3D11Device* m_pd3dDevice,
			ID3D11DeviceContext* m_pContext,
			const TCHAR* szShaderFileName = nullptr,
			const TCHAR* szTextureFileName = nullptr,
			const TCHAR* szMaskFileName = nullptr);
		virtual bool	CreateVertexBuffer();
		virtual bool    CreateIndexBuffer();
		virtual bool	CreateConstantBuffer();
		virtual bool    CreateVertexShader(const TCHAR* szFile);
		virtual bool    CreatePixelShader(const TCHAR* szFile);
		virtual bool    CreateInputLayout();
	public:
		virtual bool	Init();
		virtual bool	Frame(float fSecperFrame, float fGameTimer);
		virtual bool	Render();
		virtual bool	PreRender();
		virtual bool    Draw();
		virtual bool	PostRender();
		virtual bool	Release();
	public:
		TDxObject();
		~TDxObject();
	};

};