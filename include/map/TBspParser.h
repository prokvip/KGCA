#pragma once
#include "../TParser.h"
#include "TSpaceDivision.h"

class TBspParser : public TParser
{
public:
	ID3D11Device*   m_pd3dDevice;
	void			SetDevice( ID3D11Device*  pd3dDevice ) 
	{ 
		m_pd3dDevice = pd3dDevice; 
	}
public:
	bool		m_bBinary;
public:
	void		SetBinary( bool bBinary ) { m_bBinary = bBinary; };
	bool		Load( ID3D11Device*  pd3dDevice, TNode* pRootNode, TCHAR* szFileName, bool Binary = false);	
	bool		Save( TNode* pRootNode, TCHAR* szFileName, bool Binary = false);
	void		WriteBspTreeFile( TNode*  pBspTree );
	void		WriteBspNode( TNode*  pNode );
	void		ReadBspTreeFile( TNode*  pBspTree );
	void		ReadBspNode( TNode*  pNode );
	void		BinaryWriteBspNode( TNode*  pNode );
	void		BinaryReadBspNode( TNode*  pNode );

	bool		LoadTexture();
	void		SetBufferOpti(TNode* pNode, int iBuffer );
	void		SetNodePerRenderGroup(TNode* pNode);
	int			IsEqulVertexList( VertexList& CollectMap, PNCT_VERTEX* pTsVertex, int iPushCnt );
public:
	TBspParser(void);
	virtual ~TBspParser(void);
};
