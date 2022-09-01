#pragma once
#include "TDevice.h"
//#include "../../include/dxtk/WICTextureLoader.h"
//#include "../../include/dxtk/DDSTextureLoader.h"
//#pragma comment(lib, "../../lib/DirectXTK.lib")

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")
class Sample : public TDevice
{
	ID3D11Resource*				m_Texture	= nullptr;
	ID3D11ShaderResourceView*	m_pTextureSRV = nullptr;
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

