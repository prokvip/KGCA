#pragma once
#include "TStd.h"
#include <fbxsdk.h>
#include "TObject3D.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class TFbxObject : public TObject3D
{
public:
	std::vector<ID3D11Buffer*> m_pSubVB;
	std::vector< std::vector<PNCT_VERTEX>>   vbDataList;
	std::vector<TTexture*> m_pSubTexture;
	std::vector< W_STR >   vbTexList;
public:
	void	CreateVertexData()
	{		
	}
	HRESULT	CreateVertexBuffer()
	{
		HRESULT hr = S_OK;
		if (vbDataList.size() > 0)
		{
			for (int ivb = 0; ivb < vbDataList.size(); ivb++)
			{
				m_pSubVB.push_back(
					TDX::CreateVertexBuffer(m_pd3dDevice, 
						&vbDataList[ivb].at(0), 
					    vbDataList[ivb].size(),
					    sizeof(PNCT_VERTEX)));
			}
		}
		else
		{
			hr = TObject3D::CreateVertexBuffer();
		}
		return hr;
	}
	HRESULT CreateIndexBuffer()
	{
		return S_OK;
	}	
	bool    LoadTexture(W_STR texturename)
	{
		if (vbTexList.size() > 0)
		{
			W_STR szDefaultDir = L"../../data/fbx/";
			m_pSubTexture.resize(vbTexList.size());
			for (int ivb = 0; ivb < vbTexList.size(); ivb++)
			{
				W_STR szLoadFile = szDefaultDir + vbTexList[ivb];
				m_pSubTexture[ivb] = I_Tex.Load(szLoadFile);
			}
		}
		else
		{
			m_pTexture = I_Tex.Load(texturename);
			if (m_pTexture != nullptr)
			{
				m_pTextureSRV = m_pTexture->m_pTextureSRV;
				return true;
			}
		}
		return false;
	}
	bool	PostRender()
	{
		if (m_pIndexBuffer == nullptr)
		{
			if (vbDataList.size() > 0)
			{
				for (int iSubObj = 0; iSubObj < m_pSubVB.size(); iSubObj++)
				{
					UINT stride = sizeof(PNCT_VERTEX); // 정점1개의 바이트용량
					UINT offset = 0; // 정점버퍼에서 출발지점(바이트)
					//SLOT(레지스터리)
					m_pImmediateContext->IASetVertexBuffers(0, 1,
						&m_pSubVB[iSubObj], &stride, &offset);
					m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSubTexture[iSubObj]->m_pTextureSRV);
					m_pImmediateContext->Draw(vbDataList[iSubObj].size(), 0);
				}
			}
			else
			{
				m_pImmediateContext->Draw(m_VertexList.size(), 0);
			}
		}
		else
		{
			m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0);
		}
		return true;
	}
	bool	Release()
	{
		TObject3D::Release();

		for (int iSubObj = 0; iSubObj < m_pSubVB.size(); iSubObj++)
		{
			if (m_pSubVB[iSubObj])
			{
				m_pSubVB[iSubObj]->Release();
			}
		}
		return true;
	}
};
class TFbxLoader
{
public:
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;
	FbxNode*		m_pRootNode;
	std::vector< FbxMesh* > m_pFbxMeshList;
	std::vector< TFbxObject* > m_pDrawObjList;
	ID3D11DeviceContext* m_pContext=nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(C_STR filename);
	void PreProcess(FbxNode* pFbxNode);
	void ParseMesh(FbxMesh* pFbxMesh);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, 
								FbxLayerElementUV* VertexUVSet,
								int posIndex,
								int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh,
		FbxLayerElementVertexColor* VertexColorSet,
		int posIndex,
		int colorIndex);
	FbxVector4 ReadNormal(FbxMesh* pFbxMesh,
		FbxLayerElementNormal* VertexNormalSet,
		int posIndex,
		int colorIndex);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* MaterialSet);
};

