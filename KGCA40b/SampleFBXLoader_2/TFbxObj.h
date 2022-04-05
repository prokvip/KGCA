#pragma once
#include "TFbxImporter.h"

class TFbxObj : public TObject3D
{
public:
	TFbxImporter* m_pImporter;
	TScene m_Scene;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;
	TBoneWorld	  m_matBoneArray;
	std::vector<TFbxModel*>		 m_DrawList;	
	ID3D11Buffer*				 m_pBoneCB = nullptr;
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool    Release() override;
public:
	virtual bool	CreateConstantBuffer(ID3D11Device* pDevice);
};
