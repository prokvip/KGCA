#pragma once
#include <fbxsdk.h>
#include "TObject3D.h"
struct TAnimTrack
{
	INT      iFrame; //fTime;
	TBASIS_EX::TMatrix   matAnim;  // self * parent;
	TBASIS_EX::TMatrix   matSelfAnim; // matAnim * inv(parent)
	TBASIS_EX::TVector3     t; // self
	TBASIS_EX::TQuaternion  r; // self
	TBASIS_EX::TVector3     s; // self
};
struct TAnimScene
{
	FbxTime::EMode TimeMode;
	INT iStartFrame;
	INT iEndFrame;
	float fTickPerFrame; // 160
	float fFrameSpeed; // 30
};
struct TWeight
{
	std::vector<int>  Index;
	std::vector<float>  weight;
	void insert(int iBone, float fWeight)
	{
		for (int i = 0; i < Index.size(); i++)
		{
			if (fWeight > weight[i])
			{
				for (int j= Index.size()-1; j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iBone;
				weight[i] = fWeight;
				break;
			}
		}
	}
	TWeight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};
class TFbxObject : public TObject3D
{
public:
	UINT			   m_iObjectBone;
	TBASIS_EX::TMatrix m_matAnim;
	TBASIS_EX::TMatrix m_matControl;

	FbxAMatrix  m_fbxLocalMatrix;
	FbxNode*    m_pFbxNode		= nullptr;
	FbxNode*    m_pFbxParentNode= nullptr;
	TFbxObject* m_pParent = nullptr;
	std::vector<TFbxObject*>  m_pFbxChilds;
	std::vector<TAnimTrack>   m_AnimTracks;
	void  SetParent(TFbxObject* pParentNode)
	{
		pParentNode->m_pFbxChilds.push_back(this);
		m_pParent = pParentNode;
	}
	
public:
	std::vector<ID3D11Buffer*> m_pSubVB;
	std::vector< std::vector<PNCT_VERTEX>>   vbDataList;
	std::vector<TTexture*> m_pSubTexture;
	std::vector< W_STR >   vbTexList;
public:
	void	CreateVertexData();
	HRESULT	CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	bool    LoadTexture(W_STR texturename);
	bool	PostRender();
	bool	Release();
	TBASIS_EX::TMatrix Interplate(float fTime, TAnimScene tScene);
};

class TFbxObjectSkinning : public TFbxObject
{
public:
	bool			m_bSkinned = false;
	// sub material
	std::vector<ID3D11Buffer*> m_pSubVB_IW;
	std::vector< std::vector<IW_VERTEX>>   vbDataList_IW;
	// single model
	std::vector<IW_VERTEX>   m_VertexListIW;
	ID3D11Buffer*			 m_pVertexBufferIW;
	// skinning
	std::vector< TWeight>    m_WeightList;
	std::map<UINT, TMatrix > m_dxMatrixBindPseMap;
public:
	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	ID3D11Buffer* m_pSkinBoneCB;
public:
	HRESULT	CreateConstantBuffer()
	{
		TObject3D::CreateConstantBuffer();

		HRESULT hr;
		for (int iBone = 0; iBone < 255; iBone++)
		{
			D3DXMatrixIdentity(&m_cbDataBone.matBone[iBone]);
		}

		D3D11_BUFFER_DESC       bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(VS_CONSTANT_BONE_BUFFER) * 1; // 바이트 용량
		// GPU 메모리에 할당
		bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA  sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &m_cbDataBone;
		hr = m_pd3dDevice->CreateBuffer(
			&bd, // 버퍼 할당
			&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
			&m_pSkinBoneCB);
		return hr;
	}
	HRESULT CreateVertexLayout()
	{
		HRESULT hr;
		if (m_pVSCode == nullptr)
		{
			return E_FAIL;
		}
		// 정점레이아웃은 정점쉐이더 밀접한 관련.
		// 정점레이아웃 생성시 사전에 정점쉐이더 생성이 필요하다.
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0,40,D3D11_INPUT_PER_VERTEX_DATA, 0},

			{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,16,D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UINT NumElements = sizeof(ied) / sizeof(ied[0]);
		hr = m_pd3dDevice->CreateInputLayout(
			ied,
			NumElements,
			m_pVSCode->GetBufferPointer(),
			m_pVSCode->GetBufferSize(),
			&m_pVertexLayout);

		return hr;
	}
	HRESULT CreateVertexBuffer()
	{
		HRESULT hr= S_OK;;
		TFbxObject::CreateVertexBuffer();

		if (vbDataList_IW.size() > 0)
		{
			m_pSubVB_IW.resize(vbDataList_IW.size());
			for (int ivb = 0; ivb < vbDataList_IW.size(); ivb++)
			{
				if (vbDataList_IW[ivb].size() > 0)
				{
					m_pSubVB_IW[ivb] =
						TDX::CreateVertexBuffer(m_pd3dDevice,
							&vbDataList_IW[ivb].at(0),
							vbDataList_IW[ivb].size(),
							sizeof(IW_VERTEX));
				}
			}
		}
		else
		{
			m_pVertexBufferIW =
				TDX::CreateVertexBuffer(m_pd3dDevice,
					&m_VertexListIW.at(0),
					m_VertexListIW.size(),
					sizeof(IW_VERTEX));
		}
		return hr;		
	}	
	bool	PostRender()
	{
		/*if (m_bSkinned)
		{
			m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pSkinBoneCB);
		}*/
		if (m_pIndexBuffer == nullptr)
		{
			if (vbDataList.size() > 0)
			{
				for (int iSubObj = 0; iSubObj < m_pSubVB.size(); iSubObj++)
				{
					if (vbDataList[iSubObj].size() <= 0) continue;
					UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) }; // 정점1개의 바이트용량
					UINT offset[2] = { 0, 0 }; // 정점버퍼에서 출발지점(바이트)
					//SLOT(레지스터리)
					ID3D11Buffer* buffer[2] = { m_pSubVB[iSubObj],m_pSubVB_IW[iSubObj] };
					m_pImmediateContext->IASetVertexBuffers(0, 2, buffer, stride, offset);
					

					if (m_pSubTexture.size()> 0 && m_pSubTexture[iSubObj] != nullptr)
					{
						m_pImmediateContext->PSSetShaderResources(0, 1, &m_pSubTexture[iSubObj]->m_pTextureSRV);
					}
					m_pImmediateContext->Draw(vbDataList[iSubObj].size(), 0);
				}
			}
			else
			{
				UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) }; // 정점1개의 바이트용량
				UINT offset[2] = { 0, 0 }; // 정점버퍼에서 출발지점(바이트)
				//SLOT(레지스터리)
				ID3D11Buffer* buffer[2] = { m_pVertexBuffer,m_pVertexBufferIW };
				m_pImmediateContext->IASetVertexBuffers(0, 2, buffer, stride, offset);
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
		if (m_pSkinBoneCB)
		{
			m_pSkinBoneCB->Release();
			m_pSkinBoneCB = nullptr;
		}
		if (m_pVertexBufferIW)
		{
			m_pVertexBufferIW->Release();
			m_pVertexBufferIW = nullptr;
		}
		for (int iSubObj = 0; iSubObj < m_pSubVB_IW.size(); iSubObj++)
		{
			if (m_pSubVB_IW[iSubObj])
			{
				m_pSubVB_IW[iSubObj]->Release();
				m_pSubVB_IW[iSubObj] = nullptr;
			}
		}
		m_pSubVB_IW.clear();

		TFbxObject::Release();
		return true;
	}
};