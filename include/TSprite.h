#pragma once
#include "TShape.h"

struct TRectUV
{
	TVector4	vUV;
	RECT		Rect;
};
struct TInstatnce
{
	TMatrix matWorld;
	TVector4  uv[4];
	TVector4  color;
};
class TSprite : public TPlaneShape
{
public:
	T_STR					m_szName;
	T_STR					m_szPath;
	UINT					m_iIndex;
	UINT					m_BlendType;
	TMatrix				m_matBillboard;
	ID3D11BlendState*			m_pBlendState;
	ID3D11ShaderResourceView*	m_pAnimSRV;

	vector<int>				m_TextureIndex;
	int						m_iApplyIndex;
	int						m_iNumTexture;
	float					m_fSecPerRender;
	float					m_fTime;
	float					m_fLifeTime;
	float					m_fAnimTime; // uv  반복 시간
	float					m_fElapseTime;
	RECT					m_RectSet;
	vector<TRectUV>			m_RectList;
	TVector4				m_ParticleColor;
	
	vector<TInstatnce>			m_pInstance;
	ComPtr<ID3D11Buffer>		m_pVBInstance;
	bool						m_bInstancing;
public:
	HRESULT		SetInputLayout();
	HRESULT		CreateInstance(UINT iNumInstance);
	HRESULT		Load(	ID3D11Device* pDevice,
						TCHAR* pLoadTextureString,
						TCHAR* pLoadShaderString,
						bool   bInstancing = false,
						ID3D11BlendState* pBlendState = nullptr);
	bool		Frame(ID3D11DeviceContext*    pContext, float fGlobalTime, float fElapsedTime);
	void		Updata(ID3D11DeviceContext*    pContext, float& pfCurrentTimer, int& iApplyIndex, float fGlobalTime, float fElapsedTime);
	bool		PreDraw(ID3D11DeviceContext* pContext);
	bool		Render(ID3D11DeviceContext* pContext);
	bool		PostDraw(ID3D11DeviceContext* pContext);
	bool		RenderInstancing(ID3D11DeviceContext* pContext);
	void		SetTextureArray(T_STR_VECTOR FileList);
	TVector4	SetUV(RECT& Rect);
	void		SetUVAnimation(float fAnimTime,int iWidth=1,int iHeight=1);
	void		SetRectAnimation(
		float fAnimTime = 1.0f, 
		int iWidth = 1, 
		int iWidthSize = 0, 
		int iHeight = 1, 
		int iHeightSize = 0);
public:
	TSprite(void);
	~TSprite(void);
};

