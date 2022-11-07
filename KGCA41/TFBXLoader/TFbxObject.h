#pragma once
#include <fbxsdk.h>
#include "TObject3D.h"
struct TAnimTrack
{
	UINT      iFrame; //fTime;
	TBASIS_EX::TMatrix   matAnim;  // self * parent;
	TBASIS_EX::TMatrix   matSelfAnim; // matAnim * inv(parent)
	TBASIS_EX::TVector3     t; // self
	TBASIS_EX::TQuaternion  r; // self
	TBASIS_EX::TVector3     s; // self
};
struct TAnimScene
{
	UINT iStartFrame;
	UINT iEndFrame;
	float fTickPerFrame; // 160
	float fFrameSpeed; // 30
};
class TFbxObject : public TObject3D
{
public:
	TAnimScene  m_AnimScene;
	float       m_fAnimFrame = 0;
	float       m_fAnimInverse = 1.0f;
	float       m_fAnimSpeed = 1.0f;
	FbxAMatrix  m_fbxLocalMatrix;
	FbxNode*    m_pFbxNode		= nullptr;
	FbxNode*    m_pFbxParentNode= nullptr;
	TFbxObject* m_pParent = nullptr;
	std::vector<TFbxObject*>  m_pFbxChilds;
	std::vector<TAnimTrack>   m_AnimTracks;
	void  SetParent(TFbxObject* pParentNode)
	{
		pParentNode->m_pFbxChilds.push_back(this);
		m_pParent = pParentNode;
	}
	TBASIS_EX::TMatrix Interplate(float fTime);
public:
	std::vector<ID3D11Buffer*> m_pSubVB;
	std::vector< std::vector<PNCT_VERTEX>>   vbDataList;
	std::vector<TTexture*> m_pSubTexture;
	std::vector< W_STR >   vbTexList;
public:
	void	CreateVertexData();
	HRESULT	CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	bool    LoadTexture(W_STR texturename);
	bool	PostRender();
	bool	Release();
};