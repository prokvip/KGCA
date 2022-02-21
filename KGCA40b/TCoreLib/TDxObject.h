#pragma once
#include "TStd.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#pragma comment	(lib, "d3dcompiler.lib")
struct SimpleVertex
{
	TVector2 v;
	TVector2 t;
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
// 0001  - 기본
// 0010  - 권총
// 0100  - 장총
// 1000  - 수류탄
 
// 0111 = 7(속성)
// 1111 = 15
enum TSelectState
{
	T_DEFAULT = 0,  // 커서가 위에 없을 때(T_FOCUS상태에서 다른 곳을 T_ACTIVE하면 전환된다.)
	T_HOVER = 1,	// 커서가 위에 있을 때
	T_FOCUS = 2,	// T_ACTIVE상태에서 왼쪽 버튼을 다른 곳에서 놓았을 때(취소)
	T_ACTIVE = 4,	// 마우스 왼쪽 버튼 누르고 있을 때
	T_SELECTED = 8, // T_ACTIVE 상태에서 왼쪼버튼 놓았을 때
};
class TBaseObject
{
public:
	bool		m_bDead;
	int			m_iCollisionID;
	int			m_iSelectID;
	float		m_fSpeed;
	TVector2	m_vPos;
	TVector2	m_vDirection;
	float		m_fWidth;
	float		m_fHeight;
	TRect		m_rtCollision;
	DWORD		m_dwCollisonType;
	DWORD		m_dwSelectType;
	DWORD		m_dwSelectState;
	bool		m_bSelect;
	bool		m_bAlphaBlend = true;
public:
	virtual void	HitOverlap(TBaseObject* pObj, DWORD dwState);
	virtual void	HitSelect(TBaseObject* pObj, DWORD dwState);
public:
	
	TBaseObject()
	{
		m_bDead = false;
		m_bSelect = false;
		m_dwSelectState = T_DEFAULT;
		m_iCollisionID = -1;
		m_iSelectID = -1;
		m_vDirection.x = 0.0f;
		m_vDirection.y = 0.0f;
		m_dwCollisonType = TCollisionType::Overlap;
		m_dwSelectType = TCollisionType::Overlap;		
	}
};
struct TIndex
{
	DWORD _0;
	DWORD _1;
	DWORD _2;
};

struct TConstantData
{
	TVector4 Color;
	TVector4 Timer;
};
class TDxObject : public TBaseObject
{
public:
	
public:
	ID3D11ShaderResourceView*	m_pSRV0;
	ID3D11Texture2D*			m_pTexture0;
	ID3D11ShaderResourceView*	m_pSRV1;
	ID3D11Texture2D*			m_pTexture1;
	ID3D11BlendState*			m_AlphaBlend;
	ID3D11BlendState*			m_AlphaBlendDisable;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	std::vector<SimpleVertex> m_InitScreenList;
	std::vector<SimpleVertex> m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	
	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;

	TConstantData    m_ConstantList;
	ID3D11Buffer*    m_pConstantBuffer;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Device*		 m_pd3dDevice;	
	ID3D11DeviceContext* m_pContext;

	ID3DBlob*			m_pVSCodeResult = nullptr;
	ID3DBlob*			m_pErrorMsgs = nullptr;
	ID3DBlob*			m_pPSCodeResult = nullptr;
public:
	
	void    SetDevice(ID3D11Device* m_pd3dDevice,
					  ID3D11DeviceContext* m_pContext);
	virtual bool    LoadTexture(const TCHAR* szColorFileName,
								const TCHAR* szMaskFileName);
	virtual bool    SetVertexData();
	virtual bool    SetIndexData();
	virtual bool    SetConstantData();
	virtual bool	Create( ID3D11Device* m_pd3dDevice,
					ID3D11DeviceContext* m_pContext,					
					const TCHAR* szTextureFileName=nullptr,
					const TCHAR* szMaskFileName = nullptr);
	virtual bool	CreateVertexBuffer();
	virtual bool    CreateIndexBuffer();
	virtual bool	CreateConstantBuffer();
	virtual bool    CreateVertexShader(const TCHAR* szFile);
	virtual bool    CreatePixelShader(const TCHAR* szFile);
	virtual bool    CreateInputLayout();
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	TDxObject();
	~TDxObject();
};

