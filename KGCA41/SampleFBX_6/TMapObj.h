#pragma once
#include "TModelMgr.h"

class TMapObj : public TObject
{
	TFbxObj* m_pFbxObject; // 모델매니저의 저장된 포인터 
public:
	void		SetFbxObj(TFbxObj* pObject);
	TFbxObj*	GetFbxObj();
public:
	float   m_fCurrentAnimTime = 0.0f;
public:
	TFbxMesh* m_ptMesh;
	std::vector<const TTexture*>			m_TexArray;
	std::vector<std::shared_ptr<TMapObj>>	m_pChildObjectList;

	ID3D11Buffer* m_pVBWeightList;
	std::vector<TVertexIW>			m_pSubIWVertexList;
public:
	void    LoadTextureArray(W_STR filename);
public:
	bool	CreateVertexBuffer()	override;
	bool	CreateInputLayout()		override;	
	bool	PostRender()			override;
	//bool	Frame(float fSecPerFrame, float fGameTimer)override;
	//TBasisFBX::TMatrix Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime);
};
