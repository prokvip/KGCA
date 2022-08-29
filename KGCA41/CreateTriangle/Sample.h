#pragma once
#include "TDevice.h"

struct SimpleVertex
{
	float  x; // 위치
	float  y;
	float  z;
	//float  r; // 컬러
	//float  g;
	//float  b;
	//float  a;
};
class Sample : public TDevice
{
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
public:
	HRESULT		CreateVertexBuffer();
	HRESULT     CreateVertexLayout();
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

