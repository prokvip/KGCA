// download https://github.com/microsoft/DirectXTK
// DirectXTK_Desktop_2019.sln
// build -> DirectXTK.lib(bin)
//          inc ��� ����
// ../../inlude/DXTK -> all copy
// ../../lib -> DirectXTK.lib copy
// CreateTexture ������Ʈ -> �Ӽ� -> C/C++(�Ϲ�)-> �߰����Ե��丮 -> ../../include/dxtk;
// CreateTexture ������Ʈ -> �Ӽ� -> ��Ŀ-> �߰����̺귯�����丮 -> DirectXTK.lib;

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
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

