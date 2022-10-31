#pragma once
#include "TGameCore.h"
#include "TFbxLoader.h"
#include "TCameraDebug.h"
class Sample : public TGameCore
{
	//TFbxLoader	m_FBXLoader;
	std::vector< TFbxLoader*> m_fbxList;
	TCameraDebug* m_pMainCamera;
public:
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	bool	Release()override;
};

