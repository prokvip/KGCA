#pragma once
#include "TGameCore.h"
#include "TFbxLoader.h"
#include "TCameraDebug.h"
class Sample : public TGameCore
{
	TFbxLoader	m_FBXLoader;
	TCameraDebug* m_pMainCamera;
	TBaseObject			m_BG2;
public:
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	bool	Release()override;
};

