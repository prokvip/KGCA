#pragma once
#include <TCore.h>
#include <d3dcompiler.h>
#include "TVector.h"
#pragma comment	(lib, "D3DCompiler.lib")
struct SimpleVertex
{
	TVector3 pos;
	TVector4 color;
};
class Sample : public TCore
{
	ID3DBlob*			m_pVSBlob = nullptr;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
public:
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader();
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	Sample();
};

