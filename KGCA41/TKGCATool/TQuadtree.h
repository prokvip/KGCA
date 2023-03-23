#pragma once
#include "TNode.h"

struct SHADOW_CONSTANT_BUFFER // b1
{
	TMatrix			g_matShadow;
	TVector4        g_vTessellation;
	TVector4        g_vLightDir[3];
	TVector4		g_vLightPos[3];
	TVector4		g_vLightColor[3];
};

class TQuadtree
{
public:
	TNode* m_pRootNode;
	int		m_iMaxDepth;
	TMap* m_pLandscape = nullptr;
	TCamera* m_pCamera = nullptr;
	TTexture* m_TexArray[5];
	std::vector<TWorldObject*>   m_ObjectList;
	std::vector<TNode*> m_pLeafNodeList;
	std::vector<TNode*> m_pDrawLeafNodeList;
	
	SHADOW_CONSTANT_BUFFER			m_cbShadow;
	ComPtr<ID3D11Buffer>			m_pShadowConstantBuffer;
public:
	BYTE* m_fAlphaData;
	ComPtr<ID3D11Texture2D>  m_pMaskAlphaTex;
	ComPtr<ID3D11ShaderResourceView>  m_pMaskAlphaTexSRV;
	HRESULT CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);
	void    Splatting(TVector3 vIntersection, UINT iSplattingTexIndex, float fSplattingRadius=5.0f);
public:
	bool	Create(TCamera* pMainCamera, TMap* m_pLandscape, int iMaxDepth = 3);
	bool	AddObject(TWorldObject* pObj);
	void	BuildTree(TNode* pNode);
	bool	IsSubDivide(TNode* pNode);
	TNode* FindNode(TNode* pNode, T_BOX tBox);
	void	Reset(TNode* pNode);
	UINT    SelectVertexList(T_BOX& tBox, std::vector<TNode*>& selectNodeList);
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool	Release();
	bool    RenderShadow(ID3D11DeviceContext* pContext, TCamera* pCamera);
	void	RenderShadowObject(ID3D11DeviceContext* pContext, TNode* pNode);
	void    RenderObject(ID3D11DeviceContext* pContext, TNode* pNode);
	void    VisibleNode(TNode* pNode);
	virtual ~TQuadtree();
};

