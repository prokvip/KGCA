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
	//  오브젝트 로드 관련 함수
	////////////////////////////////////////////////
	int			GetMeshCounter() { return (int)m_pData.size(); }
	bool		Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread = false);
	//  씬 정보 로드
	bool		LoadScene( const TCHAR* strFileName );
	//  메터리얼 정보 로드
	bool		LoadMaterial();
	void		LoadMaterialHeader(TMtrl* pMtrl);
	void		LoadTexMap(	TMtrl* pMtrl,T_STR szDirName );
	//  오브젝트 정보 로드
	bool		LoadObject();
	int			LoadVertexIndex(tTbsData* pMesh);
	//  에니메이션 정보 로드
	void		LoadAnimation(TMesh* pMesh);
	bool		LoadAnimationTrack( int iNumTrack, vector<shared_ptr<TAnimTrack>>& pTrackList );
	//--------------------------------------------------------------------------------------
	// 랜더링 관련 정보 생성
	//--------------------------------------------------------------------------------------
	// 로딩 된 정보를 TMesh로 변환
	bool		Convert(ID3D11Device* pd3dDevice);
	// 매터리얼 세팅
	bool		SetMaterial();
	int			GetMapID( TMtrl* pMtrl, int iTexMapType=ID_TBASIS_DI ); // 맵 인덱스 검색
    // 상속관계 구축
	bool		InheriteCollect();
	TMesh*		SearchToCollects(T_STR	m_strParentName );
	//--------------------------------------------------------------------------------------
	// 에니메이션 보간 관련 함수 
	//--------------------------------------------------------------------------------------
	TAnimTrack*	SetDoublyLinkedList( TAnimTrack* pCurrentTrack, TAnimTrack* pPrev );//이중 연결 리스트 구축
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
