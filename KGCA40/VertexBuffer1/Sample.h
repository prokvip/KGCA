#pragma once
#include <TCore.h>
#include <d3dcompiler.h>
#include "XVector.h"
#pragma comment	(lib, "D3DCompiler.lib")
struct SimpleVertex
{
	XVector3 pos;
	XVector4 color;
	SimpleVertex(XVector3 p, XVector4 c)
	{
		pos = p;
		color = c;
	}
	SimpleVertex()
	{
		pos = { 0.5f,0.5f,0.5f };
		color = { 0.5f,0.5f,0.5f,0.5f };
	}
};
class Sample : public TCore
{
	std::vector< SimpleVertex> m_VertexList;
	ID3DBlob*			m_pVSBlob = nullptr;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
public:
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader();
	bool		LoadObject(std::wstring filename);
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	Sample();
};

