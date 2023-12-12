#include "pch.h"
#include "TFbxObject.h"

bool	TFbxObj::CreateConstantBuffer()
{
	TDxObject::CreateConstantBuffer();

	HRESULT hr;
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TBoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//D3D11_SUBRESOURCE_DATA sd;
	//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//sd.pSysMem = &m_matBoneArray;

	if (FAILED(hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pBoneCB)))
	{
		return false;
	}

	
	return true;
}
void TFbxObj::SetAnim(UINT startFrame, UINT endFrame, UINT frameSpeed, UINT tickForFrame)
{
	m_iStartFrame = startFrame;
	m_iEndFrame = endFrame;
	m_iFrameSpeed = frameSpeed;
	m_iTickForFrame = tickForFrame;
}
UINT    TFbxObj::GetStartFrame()
{
	return m_iStartFrame;
}
UINT    TFbxObj::GetEndFrame()
{
	return m_iEndFrame;
}
UINT    TFbxObj::GetFrameSpeed()
{
	return m_iFrameSpeed;
}
UINT    TFbxObj::GetTickForFrame()
{
	return m_iTickForFrame;
}

void     TFbxObj::LoadTextureArray(W_STR filename)
{
	m_TexArray.push_back(I_Tex.Load(filename));
}

bool	TFbxObj::CreateVertexBuffer()
{
	TDxObject::CreateVertexBuffer();

	HRESULT hr;

	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.ByteWidth = sizeof(TVertexIW) * m_pVertexListIW.size();
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(InitialData));
	InitialData.pSysMem = &m_pVertexListIW.at(0);

	hr = m_pDevice->CreateBuffer(
		&Desc,
		&InitialData,
		&m_pVBWeightList);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TFbxObj::CreateInputLayout()
{
	// 정점쉐이더의 결과를 통해서 정점레이아웃을 생성한다.	
	// 정점버퍼의 각 정점의 어떤 성분을 정점쉐이더에 전달할 거냐
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0,40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"INDEX",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENT",0, DXGI_FORMAT_R32G32B32_FLOAT, 1,32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pDevice->CreateInputLayout(
		layout,
		NumElements,
		m_pShader->GetBufferPointer(),
		m_pShader->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}