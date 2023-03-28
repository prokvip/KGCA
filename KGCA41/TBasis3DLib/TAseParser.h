#pragma once
#include "TObjStd.h"
#include "TLoadToken.h"


static const TCHAR* AseSections[] = { _T("SCENE "),_T("MATERIAL_LIST "),_T("GEOMOBJECT "),_T("HELPEROBJECT "), };
enum AseSectionType { SCENE = 0, MATERIALLIST, GEOMOBJECT, HELPEROBJECT, };

static const TCHAR* AseMaterialTokens[] = { _T("NUMSUBMTLS"),	_T("MAP_NAME"),_T("}"), };
enum AseMaterialType { NUM_SUBMTRLS = 0, MAP_NAME, };

static const TCHAR* AseTextureTokens[] = { _T("MAP_SUBNO"), _T("BITMAP "),
	_T("SUBMATERIAL"),  // Return
	_T("MATERIAL "),// Return
	_T("HELPEROBJECT "), // Return
	_T("GEOMOBJECT ") };// Return
enum AseTextureType { MAP_SUBNO =0, BITMAP_FILE=1, };

static const TCHAR* AseGeomObjectTokens[] = {
	_T("NODE_PARENT"),
	_T("NODE_TM"),
	_T("MESH {"),
	_T("TM_ANIMATION"),
	_T("NODE_VISIBILITY_TRACK"),
	_T("MATERIAL_REF"),

	_T("GEOMOBJECT"),	// Return
	_T("HELPEROBJECT"),// Return
};
enum AseGeomObjectType {
	NODE_PARENT = 0, MODE_TM,
	MESH, TM_ANIMATION, TM_VISIBILITY, MATERIAL_REF,
};

static const TCHAR* AseHelperObjectTokens[] = {
	_T("NODE_PARENT"),
	_T("HELPER_CLASS"),
	_T("NODE_TM"),
	_T("BOUNDINGBOX_MIN"),
	_T("TM_ANIMATION"),

	_T("GEOMOBJECT"),	// Return
	_T("HELPEROBJECT"),// Return

};
enum AseHelperObjectType {
	HELPER_NODE_PARENT = 0, HELPER_CLASS, HELPER_MODE_TM,
	BOUNDINGBOX_MIN, HELPER_TM_ANIMATION,
};

enum AseTrackType {
	POS_SAMLE_TRACK = 40, ROT_SAMLE_TRACK, SCL_SAMLE_TRACK, VIS_SAMLE_TRACK
};


struct TAseMesh
{
	TMatrix				m_matWorld;
	T_STR					m_strNodeName;
	DWORD					m_iNumFace;
	INT						m_iMtrlRef;
	TVertexList				m_PosVertexList;
	TVertexList				m_TexVertexList;
	TVertexList				m_ColVertexList;
	TVertexList				m_NorVertexList;

	TMatrix				m_matWorldTrans;	// 월드 이동 행렬
	TMatrix				m_matWorldRotate;	// 월드 회전 행렬
	TMatrix				m_matWorldScale;	// 월드 신축 행렬

	vector<TAnimTrack>		m_PosTrack;		// 이동 트랙 
	vector<TAnimTrack>		m_RotTrack;		// 회전 트랙
	vector<TAnimTrack>		m_SclTrack;		// 신축 트랙												
	vector<TAnimTrack>		m_VisTrack;		// 알파 트랙
	OBJECTCLASSTYPE			m_ClassType;		// 본 : 더미 : 지오메트리 지정	
	T_STR					m_strParentName;	// 부모이름
	T_BOX					m_BBox;
	TAseMesh()
	{
		m_iNumFace = 0;
		m_iMtrlRef = -1;
	}
};
class TAseParser : public TLoadToken
{
public:
	vector<shared_ptr<TAseMesh>>	m_pAseMesh;
public:
	bool			Release();
	bool			Load(const TCHAR* strFileName);
	bool			LoadScene(TScene& tScene);
	bool			LoadMaterialList(vector<TMtrl>&	tMaterialList);
	bool			LoadMaterial(TMtrl* pMaterial);
	bool			LoadTexture(TMtrl*	pMaterial);
	bool			LoadGeomObject();
	bool			LoadHelperObject();
	void			LoadTM(TAseMesh* pObject);
	void			LoadMesh(TAseMesh* pObject);
	void			LoadAnimation(TAseMesh* pObject);
	void			LoadVisAnimation(TAseMesh* pObject);
public:
	bool			GetData(const TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA, bool bLoop = true);
	bool			GetData(VOID* pData = NULL, INT DataType = NULL_DATA);
	bool			GetVertexFromIndexData(const TCHAR* strToken, DWORD dwSearchIndex, VOID* pData, INT DataType);
	bool			GetVertexListFromString(const TCHAR* strVertexList, DWORD dwNumVertex, vector<TVector3>& VertexList, const TCHAR* strToken = _T("*"));
	bool			GetFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TVertexList& VertexList, const TCHAR* strToken = _T("*"));
	bool			GetPositionFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TVertexList& VertexList, const TCHAR* strToken = _T("*"));
	bool			GetNormalListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TAseMesh* pGeomObject, const TCHAR* strToken = _T("*"));
	bool			GetTrackListFromString(vector<TAnimTrack>& PosTrack,
		AseTrackType TrackType);

public:
	TAseParser();
	virtual ~TAseParser();
};