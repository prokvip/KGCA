#pragma once
#include "TGameCore.h"
#include "TFbxFile.h"
#include "TCameraDebug.h"
#include "TCharacter.h"

class Sample : public TGameCore
{
	//TFbxFile	m_FBXLoader;
	std::vector< TFbxFile*> m_fbxList;
	std::vector< TCharacter*> m_NpcList;
	TCharacter* m_UserCharacter;
	TCameraDebug* m_pMainCamera;
public:
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	bool	Release()override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};
