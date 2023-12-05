#pragma once
#include "TCore.h"
#include "TMap.h"
#include "TDebugCamera.h"
#include "TQuadtree.h"
#include "TModelMgr.h"

class TMapObject : public TObject
{
	TFbxObj* m_pFbxObject; // �𵨸Ŵ����� ����� ������ 
public:	
	void		SetFbxObj(TFbxObj* pObject);
	TFbxObj*	GetFbxObj();
public:
	float   m_fCurrentAnimTime = 0.0f;
public:
	TFbxMesh* m_ptMesh;
	std::vector<const TTexture*>			m_TexArray;
	std::vector<std::shared_ptr<TMapObject>>m_pChildObjectList;
	bool	CreateInputLayout() override;
	bool	CreateVertexBuffer() override;
	std::vector<ID3D11Buffer*>   m_pVBWeightList;
public:
	bool	CreateVertexBuffer()
	{
		TDxObject::CreateVertexBuffer();
		//for (int iMtrl = 0; iMtrl < m_pSubIWVertexList.size(); iMtrl++)
		//{
		//	if (m_pSubIWVertexList[iMtrl].size() <= 0) continue;

		//	//gpu�޸𸮿� ���� �Ҵ�(���ϴ� �Ҵ� ũ��)
		//	D3D11_BUFFER_DESC bd;
		//	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		//	bd.ByteWidth = sizeof(TVertexIW) * m_pSubIWVertexList[iMtrl].size();
		//	bd.Usage = D3D11_USAGE_DEFAULT;
		//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//	D3D11_SUBRESOURCE_DATA sd;
		//	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		//	sd.pSysMem = &m_pSubIWVertexList[iMtrl].at(0);

		//	if (FAILED(hr = m_pd3dDevice->CreateBuffer(&bd, &sd,
		//		&m_pVBWeightList[iMtrl])))
		//	{
		//		return false;
		//	}
		//}
	}
	bool	CreateInputLayout()
	{
		TDxObject::CreateInputLayout();

		// �������̴��� ����� ���ؼ� �������̾ƿ��� �����Ѵ�.	
		// ���������� �� ������ � ������ �������̴��� ������ �ų�
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
	void     LoadTextureArray(W_STR filename)
	{
		m_TexArray.push_back(I_Tex.Load(filename));
	}	
	bool   PostRender()
	{
		for (int isubMtrl = 0; isubMtrl < m_ptMesh->m_TriangleList.size(); isubMtrl++)
		{
			if (m_TexArray.size() && m_TexArray[isubMtrl])
			{
				m_TexArray[isubMtrl]->Apply(m_pImmediateContext, 0);
			}
			if (m_pIndexBuffer == nullptr)
			{
				m_pImmediateContext->Draw(m_ptMesh->m_TriangleList[isubMtrl].size(), 
									      m_ptMesh->m_TriangleOffsetList[isubMtrl]);
			}			
		}
		return true;
	}
};


class Sample : public TCore
{
	using NEW_OBJECT = std::shared_ptr<TMapObject>;
	NEW_OBJECT		m_MapObj;
	const TTexture* m_pTex;
	std::shared_ptr<TDebugCamera>  m_pDebugCamera;	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	bool  DeleteDxResource() override;
	bool  CreateDxResource() override;
};

