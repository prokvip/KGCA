#pragma once
#include "TNode.h"
typedef vector<TNode*>	TNODE_VECTOR;

class TMapLOD
{
public:
	ID3D11Device*			m_pd3dDevice;
	TCamera*				m_pCamera;
	vector<TNODE_VECTOR>	m_LevelList;	
	vector< TNode* >		m_DrawPatchNodeList;
	vector< TNode* >		m_DrawNodeList;	
	// LOD ����(  ȭ�� ���� ����( 0 ~ 1 ���� ��� )
	float					m_fExpansionRatio;	
	// Ʈ���� ������ ���̿� ���� �� ����
	int						m_iNumCell;	
	// LOD ���� ����( z = pow( x,y )���� y = log(z) / log(x) ) 
	int						m_iPatchLodCount;
	bool					m_bThresHoldValue; // ���� �� ���
	void					SetThresHold( bool bFlag ) {m_bThresHoldValue=bFlag; };

public:	
	virtual void	InitLevelOrder( TNode* pRootNode,int iMaxDepth );
	virtual void	SetLOD(  DWORD dwWidth, int iNumDepth );
	virtual void	SetExpansionRatio( int iRatio ) { m_fExpansionRatio=(float)iRatio; }
	virtual DWORD	GetLodSubIndex(TNode* pNode );
	virtual DWORD	GetLodType(TNode* pNode);
	virtual float	GetExpansionRatio( TVector3 vCenter, float fRadius );
	virtual float	GetExpansionRatio( TVector3 vMax, TVector3 vMin );
	virtual float	GetExpansionRatio( TVector3 vCenter );
	virtual void	GetDrawPatchNode();
	virtual void	AddDrawPatchNode( TNode* pNode );
	virtual void	Update( ID3D11Device* pd3dDevice, TCamera* pCamera = NULL);	

public:
	TMapLOD(void);
	virtual ~TMapLOD(void);
};
