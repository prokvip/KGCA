#pragma once
#include "TNode.h"

class TQuadtree
{
public:
	TNode* m_pRootNode;
	int		m_iMaxDepth;
	TMap* m_pMap = nullptr;
	TCamera* m_pCamera = nullptr;
	TTexture* m_TexArray[5];
	std::vector<TWorldObject*>   m_ObjectList;
	std::vector<TNode*> m_pLeafNodeList;
	std::vector<TNode*> m_pDrawLeafNodeList;
public:
	BYTE* m_fAlphaData;
	ComPtr<ID3D11Texture2D>  m_pMaskAlphaTex;
	ComPtr<ID3D11ShaderResourceView>  m_pMaskAlphaTexSRV;
	HRESULT CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);
	void    Splatting(TVector3 vIntersection, UINT iSplattingTexIndex, float fSplattingRadius=5.0f);
public:
	bool	Create(TCamera* pMainCamera, TMap* m_pMap, int iMaxDepth = 3);
	bool	AddObject(TWorldObject* pObj);
	void	BuildTree(TNode* pNode);
	bool	IsSubDivide(TNode* pNode);
	TNode* FindNode(TNode* pNode, T_BOX tBox);
	void	Reset(TNode* pNode);
	UINT    SelectVertexList(T_BOX& tBox, std::vector<TNode*>& selectNodeList);
	bool	Frame();
	bool	Render();
	bool	Release();
	bool    RenderShadow(TCamera* pCamera);
	void	RenderShadowObject(TNode* pNode);
	void    RenderObject(TNode* pNode);
	void    VisibleNode(TNode* pNode);
	virtual ~TQuadtree();
};

