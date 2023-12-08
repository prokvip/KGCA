#pragma once
#include "TModelMgr.h"

class TMapObj 
{
public:
	TFbxObj*		m_pModel; // 모델매니저의 저장된 포인터 
	TFbxObj*		m_pActionModel; // 모델매니저의 저장된 포인터 
public:
	float			m_fCurrentAnimTime = 0.0f;
	TBoneWorld		m_matBoneArray;
	ID3D11Buffer*	m_pBoneCB = nullptr;
	TMatrix			m_matControl;
public:
	virtual bool			Release() { return true; };
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			CreateBoneBuffer();
	//bool	Frame(float fSecPerFrame, float fGameTimer)override;
	//TBasisFBX::TMatrix Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime);
};


class TMapObjSkinning : public TMapObj
{
public:
	bool			Release() { return true; };
	bool			Frame();
	bool			Render();
	bool			CreateBoneBuffer();
	void			AllNodeRender();
};