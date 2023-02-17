#include "pch.h"
#include "TMap.h"
float TMap::GetHeightmap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y;
}
float TMap::GetHeight(float fPosX, float fPosZ)
{
	
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
	// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fCellX = (float)(m_iNumCellCols * m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows * m_fCellDistance / 2.0f - fPosZ);

	// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// 보간작업를 위한 기준 페잇스를 찾는다. 
	float fHeight = 0.0f;
	// 윗페이스를 기준으로 보간한다.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래페이스를 기준으로 보간한다.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
float TMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}
bool  TMap::Build(UINT iWidth, UINT iHeight)
{
	// 전체 셀 개수 = (iWidth-1)*(iHeigth-1)
	// 전체 페이스 개수 = (iWidth-1)*(iHeigth-1)*2
	// 전체 정점 개수 = iWidth * iHeight
	m_iNumRows = iWidth;
	m_iNumCols= iHeight;
	m_iNumCellCols = m_iNumCols-1;
	m_iNumCellRows = m_iNumRows-1;
	m_VertexList.resize(iWidth * iHeight);
	int iHalfWidth = iWidth /2;
	int iHalfHeight = iHeight/2;

	for (int iRow = 0; iRow < iHeight; iRow++)
	{
		for (int iCol = 0; iCol < iWidth; iCol++)
		{
			m_VertexList[iRow * iWidth + iCol].p =
				//{(float)iCol, 0.0f, (float)iHeight-iRow};
			{	(float)(iCol- iHalfWidth)* m_fCellDistance, 
				0.0f,//cosf(DegreeToRadian(iCol))*20.0f+ sinf(DegreeToRadian(iRow)) * 20.0f,
				(float)(iHalfHeight- iRow)* m_fCellDistance };
			if(m_fHeightList.size() > 0)
				m_VertexList[iRow * iWidth + iCol].p.y = m_fHeightList[iRow*m_iNumCols+iCol]* m_fScaleHeight;
			else
				m_VertexList[iRow * iWidth + iCol].p.y = 0.0f;
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

	m_iNumRows = td.Height;
	m_iNumCols = td.Width;
	pTexture2D->Release();
	return true;
}

void TMap::GenVertexNormal()
{	
	m_FaceNormals.resize(m_dwFace);
	UINT iFace = 0;
	for( UINT i=0; i < m_IndexList.size(); i+=3)
	{
		UINT i0 = m_IndexList[i+0];
		UINT i1 = m_IndexList[i + 1];
		UINT i2 = m_IndexList[i + 2];
		m_FaceNormals[iFace].vertexArray[0] = i0;
		m_FaceNormals[iFace].vertexArray[1] = i1;
		m_FaceNormals[iFace].vertexArray[2] = i2;
		m_FaceNormals[iFace++].vNormal = ComputeFaceNormal(i0, i1, i2);
	}
	
	m_VertexInfo.resize(m_iNumCols* m_iNumRows);
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
		ComputeVertexNormal(iVertex);
	}
}
void TMap::ComputeVertexNormal(UINT iVertex)
{

	for (UINT i = 0; i < m_VertexInfo[iVertex].faceIndexArray.size(); i++)
	{
		UINT faceindex = m_VertexInfo[iVertex].faceIndexArray[i];
		UINT i0 = m_FaceNormals[faceindex].vertexArray[0];
		UINT i1 = m_FaceNormals[faceindex].vertexArray[1];
		UINT i2 = m_FaceNormals[faceindex].vertexArray[2];
		m_FaceNormals[faceindex].vNormal = ComputeFaceNormal(i0, i1, i2);

		m_VertexInfo[iVertex].vNormal += m_FaceNormals[faceindex].vNormal;
	}
	D3DXVec3Normalize(&m_VertexInfo[iVertex].vNormal,
		&m_VertexInfo[iVertex].vNormal);

	m_VertexList[iVertex].n = m_VertexInfo[iVertex].vNormal;
#ifdef _DEBUG
	TVector3 vLight = { -1, 0, 0 };
	TVector3 vTarget = { 0, 0, 0 };
	TVector3 vDir = vTarget - vLight;
	D3DXVec3Normalize(&vLight, &vLight);
	float fDot = D3DXVec3Dot(&-vLight, &m_VertexList[iVertex].n);
	m_VertexList[iVertex].c = { fDot+0.3f ,fDot + 0.3f ,fDot + 0.3f , 1 };
#endif
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