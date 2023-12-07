#pragma once
#include "TModelMgr.h"

class TMapObj 
{
	TFbxObj*	m_pFbxObject; // �𵨸Ŵ����� ����� ������ 
public:
	void		SetFbxObj(TFbxObj* pObject);
	TFbxObj*	GetFbxObj();
public:
	float			m_fCurrentAnimTime = 0.0f;
	TBoneWorld		m_matBoneArray;
	ID3D11Buffer*	m_pBoneCB = nullptr;
	TMatrix			m_matWorld;
public:
	bool			Release() { return true; };
	//bool	Frame(float fSecPerFrame, float fGameTimer)override;
	//TBasisFBX::TMatrix Interplate(TFbxImporter* pAnimImp, TFbxModel* pModel, float fTime);
};
