#pragma once
#include "TFbxImporter.h"

class TFbx :public TObject3D
{
public:
	TFbxImporter* m_pImporter;
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
};
