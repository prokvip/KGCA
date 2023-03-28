#pragma once
#include "TMesh.h"
#include "TParser.h"

typedef TData<PNCT_VERTEX>				tTbsData;
typedef vector<shared_ptr<tTbsData>>	tTbsMeshData;

class TTbsObj : public TActor
{
public:	
	tTbsMeshData	m_pData;
	TParser			m_Parser;
	vector<TMtrl>	m_Material;	
	DWORD			m_dwNumChildren;
public:
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	virtual bool		Draw(ID3D11DeviceContext*    pContext, TActor* pParent);
	bool		Release();
	bool		ResetResource();
	////////////////////////////////////////////////
	//  ������Ʈ �ε� ���� �Լ�
	////////////////////////////////////////////////
	int			GetMeshCounter() { return (int)m_pData.size(); }
	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	//  �� ���� �ε�
	bool		LoadScene( const TCHAR* strFileName );
	//  ���͸��� ���� �ε�
	bool		LoadMaterial();
	void		LoadMaterialHeader(TMtrl* pMtrl);
	void		LoadTexMap(	TMtrl* pMtrl,T_STR szDirName );
	//  ������Ʈ ���� �ε�
	bool		LoadObject();
	int			LoadVertexIndex(tTbsData* pMesh);
	//  ���ϸ��̼� ���� �ε�
	void		LoadAnimation(TMesh* pMesh);
	bool		LoadAnimationTrack( int iNumTrack, vector<shared_ptr<TAnimTrack>>& pTrackList );
	//--------------------------------------------------------------------------------------
	// ������ ���� ���� ����
	//--------------------------------------------------------------------------------------
	// �ε� �� ������ TMesh�� ��ȯ
	bool		Convert(ID3D11Device* pd3dDevice);
	// ���͸��� ����
	bool		SetMaterial();
	int			GetMapID( TMtrl* pMtrl, int iTexMapType=ID_TBASIS_DI ); // �� �ε��� �˻�
    // ��Ӱ��� ����
	bool		InheriteCollect();
	TMesh*		SearchToCollects(T_STR	m_strParentName );
	//--------------------------------------------------------------------------------------
	// ���ϸ��̼� ���� ���� �Լ� 
	//--------------------------------------------------------------------------------------
	TAnimTrack*	SetDoublyLinkedList( TAnimTrack* pCurrentTrack, TAnimTrack* pPrev );//���� ���� ����Ʈ ����
	bool		GetAnimationTrack(	float fFrame,vector<shared_ptr<TAnimTrack>> pTrackList,
									TAnimTrack** ppStartTrack, 
									TAnimTrack** ppEndTrack );
	TMatrix	Interpolate(TMesh* pMesh,TMatrix* matParent, float fFrameTick );
	bool		UpdateBuffer();
	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
public:
	TTbsObj(void);
	virtual ~TTbsObj(void);
};
