#include "TMap.h"
bool  TMap::Build(UINT iWidth, UINT iHeight)
{
	// 전체 셀 개수 = (iWidth-1)*(iHeigth-1)
	// 전체 페이스 개수 = (iWidth-1)*(iHeigth-1)*2
	// 전체 정점 개수 = iWidth * iHeight
	m_dwNumRows = iWidth;
	m_dwNumColumns= iHeight;
	m_VertexList.resize(iWidth * iHeight);
	int iHalfWidth = iWidth /2;
	int iHalfHeight = iHeight/2;
	float fCellDistance = 1.0f;
	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iCol = 0; iCol < iWidth; iCol++)
		{
			m_VertexList[iRow * iWidth + iCol].p =
				//{(float)iCol, 0.0f, (float)iHeight-iRow};
			{	(float)(iCol- iHalfWidth)* fCellDistance, 
				0.0f,//cosf(DegreeToRadian(iCol))*20.0f+ sinf(DegreeToRadian(iRow)) * 20.0f,
				(float)(iHalfHeight- iRow)* fCellDistance };
			m_VertexList[iRow * iWidth + iCol].p.y = m_fHeightList[iRow*m_dwNumColumns+iCol]*100.0f;
			m_VertexList[iRow * iWidth + iCol].c =	{ 1,1,1,1 };
			m_VertexList[iRow * iWidth + iCol].t =	{ ((float)iCol / (float)(iWidth-1))*10.0f,
													 ((float)iRow / (float)(iHeight - 1))* 10.0f };
		}
	}

	m_IndexList.resize((iWidth - 1) * (iHeight - 1) * 2 * 3.0f);
	UINT iIndex = 0;
	for (int iRow = 0; iRow < iHeight-1; iRow++)
	{
		for (int iCol = 0; iCol < iWidth-1; iCol++)
		{
			UINT iNextRow = iRow + 1;
			UINT iNextCol = iCol + 1;
			m_IndexList[iIndex + 0] = iRow*iWidth+iCol;
			m_IndexList[iIndex + 1] = m_IndexList[iIndex + 0]+1;
			m_IndexList[iIndex + 2] = iNextRow*iWidth+iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 3]+1;

			iIndex += 6;
		}
	}
	indexlist.resize(m_IndexList.size());
	m_dwFace = m_IndexList.size() / 3;

	GenVertexNormal();
	return true;
}
bool TMap::UpdateBuffer(TCameraDebug* pMainCamera)
{	
	m_dwFace = 0;
	DWORD index = 0;
	TVector3 v[3];
	for (int iFace = 0; iFace < m_IndexList.size() / 3; iFace++)
	{
		UINT i0 = m_IndexList[iFace * 3 + 0];
		UINT i1 = m_IndexList[iFace * 3 + 1];
		UINT i2 = m_IndexList[iFace * 3 + 2];
		v[0] = m_VertexList[i0].p;
		v[1] = m_VertexList[i1].p;
		v[2] = m_VertexList[i2].p;
		for (int i = 0; i < 3; i++)
		{
			bool bRender = pMainCamera->m_vFrustum.ClassifyPoint(v[i]);
			if (bRender)
			{
				indexlist[index++] = i0;
				indexlist[index++] = i1;
				indexlist[index++]= i2;
				m_dwFace++;
				break;
			}
		}
	}
	m_pImmediateContext->UpdateSubresource(
		m_pIndexBuffer, 0, nullptr,
		&indexlist.at(0), 0, 0);
	return true;
}

bool TMap::LoadHeightMap(ID3D11Device* pd3dDevice,// 디바이스 객체
	ID3D11DeviceContext* pContext, W_STR loadTexture)
{

	ComPtr<ID3D11Resource> pTexture;
	HRESULT hr= DirectX::CreateWICTextureFromFileEx(pd3dDevice, loadTexture.c_str(),
		0, D3D11_USAGE_STAGING, 0,
		D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, 0,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), nullptr);

	ID3D11Texture2D* pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC td;
	pTexture2D->GetDesc(&td);

	m_fHeightList.resize(td.Height * td.Width);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pContext->Map(pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0,
		&MappedFaceDest)))
	{
		UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
		for (UINT row = 0; row < td.Height; row++)
		{
			UINT rowStart = row * MappedFaceDest.RowPitch;
			for (UINT col = 0; col < td.Width; col++)
			{
				UINT colStart = col * 4;
				UINT uRed = pTexels[rowStart+colStart+0];
				m_fHeightList[row*td.Width+col] = (float)uRed/255.0f;
			}
		}
		pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}

	m_dwNumRows = td.Height;
	m_dwNumColumns = td.Width;
	pTexture2D->Release();
	return true;
}

void TMap::GenVertexNormal()
{
	std::vector<TVector3> m_FaceNormals;
	m_FaceNormals.resize(m_dwFace);
	UINT iFace = 0;
	for( UINT i=0; i < m_IndexList.size(); i+=3)
	{
		UINT i0 = m_IndexList[i+0];
		UINT i1 = m_IndexList[i + 1];
		UINT i2 = m_IndexList[i + 2];
		m_FaceNormals[iFace++] = ComputeFaceNormal(i0, i1, i2);
	}
	struct tVertexInfo
	{
		std::vector<UINT >  faceIndexArray;		
		TVector3 vNormal;
	};
	std::vector<tVertexInfo> m_VertexInfo;
	m_VertexInfo.resize(m_dwNumColumns* m_dwNumRows);
	for (UINT iFace = 0; iFace < m_dwFace; iFace ++)
	{
		for (UINT i = 0; i < 3; i++)
		{
			UINT i0 = m_IndexList[iFace*3+i];
			m_VertexInfo[i0].faceIndexArray.push_back(iFace);
		}
	}
	for (UINT iVertex = 0; iVertex < m_VertexInfo.size(); iVertex++)
	{
		for (UINT i = 0; i < m_VertexInfo[iVertex].faceIndexArray.size(); i++)
		{
			m_VertexInfo[iVertex].vNormal += 
				m_FaceNormals[m_VertexInfo[iVertex].faceIndexArray[i]];
		}
		D3DXVec3Normalize(&m_VertexInfo[iVertex].vNormal, 
						  &m_VertexInfo[iVertex].vNormal);

		m_VertexList[iVertex].n = m_VertexInfo[iVertex].vNormal;
#ifdef _DEBUG
		TVector3 vLight = { 0, 1, 0 };
		float fDot = D3DXVec3Dot(&vLight, &m_VertexList[iVertex].n);
		m_VertexList[iVertex].c = { fDot ,fDot ,fDot , 1 };
#endif
	}
}
TVector3 TMap::ComputeFaceNormal(UINT i0, UINT i1, UINT i2)
{
	TVector3 vNormal;
	TVector3 e0 = m_VertexList[i1].p - m_VertexList[i0].p;
	TVector3 e1 = m_VertexList[i2].p - m_VertexList[i0].p;
	D3DXVec3Cross(&vNormal, &e0, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}