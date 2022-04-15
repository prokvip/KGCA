#pragma once
#include "TFbxImporter.h"
#include "TCamera.h"
class TFbx :public TObject3D
{
public:
	TCamera* m_pMainCamera;
public:
	TFbxImporter* m_pMeshImp;
	TFbxImporter* m_pAnimImporter;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;
	TBoneWorld			m_matBoneArray;
	std::vector<TFbx>	m_DrawList;
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool    Release() override;
	void			GenAABB()override;
public:
	
	T::TMatrix Interplate(TFbxImporter* pAnimImp,TFbxModel* pModel, float fFrame);
};
