#include "TMap.h"
DXGI_FORMAT TMap::MAKE_TYPELESS(DXGI_FORMAT format)
{
	/*  if( !DXUTIsInGammaCorrectMode() )
	return format;*/

	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;

	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_TYPELESS;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_TYPELESS;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_TYPELESS;
	};

	return format;
}
//--------------------------------------------------------------------------------------
// Helper functions to create SRGB formats from typeless formats and vice versa
//--------------------------------------------------------------------------------------
DXGI_FORMAT TMap::MAKE_SRGB(DXGI_FORMAT format)
{
	/*  if( !DXUTIsInGammaCorrectMode() )
	return format;*/

	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	};
	return format;
}
float TMap::GetHeight(float fPosX, float fPosZ)
{
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
	// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fCellX = (float)(m_iNumCellCols*m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows*m_fCellDistance / 2.0f - fPosZ);

	// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2)< fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2)< fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

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
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}
TVector3 TMap::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	TVector3 vNormal;
	TVector3 v0 = m_VertexList[dwIndex1].p - m_VertexList[dwIndex0].p;
	TVector3 v1 = m_VertexList[dwIndex2].p - m_VertexList[dwIndex0].p;

	D3DXVec3Cross(&vNormal, &v0, &v1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}
HRESULT TMap::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice, m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
HRESULT TMap::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iNumVertex = m_iNumVertices;
	m_dxobj.g_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice, &m_VertexList.at(0), m_dxobj.m_iNumVertex, m_dxobj.m_iVertexSize));
	return hr;
}
HRESULT TMap::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	m_dxobj.m_iNumIndex = m_iNumFace * 3;//(m_iNumRows - 1)*(m_iNumCols - 1) * 2 * 3
	m_dxobj.g_pIndexBuffer.Attach(DX::CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_dxobj.m_iNumIndex, sizeof(DWORD)));
	return hr;
}
bool TMap::CreateVertexData()
{
	m_VertexList.resize(m_iNumVertices);

	float fHalfCols = (m_iNumCols-1) / 2.0f;
	float fHalfRows = (m_iNumRows-1) / 2.0f;
	float ftxOffsetU = 1.0f / (m_iNumCols - 1);
	float ftxOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{ 
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].p.x = (iCol - fHalfCols)		* m_fCellDistance;
			m_VertexList[iVertexIndex].p.z = -((iRow - fHalfRows)	* m_fCellDistance);
			m_VertexList[iVertexIndex].p.y =  GetHeightOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].n = GetNormalOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].c = GetColorOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].t = GetTextureOfVertex( ftxOffsetU * iCol, ftxOffsetV * iRow);
		}
	}
	return true;
}
bool TMap::CreateIndexData()
{
	m_IndexList.resize(m_iNumFace * 3);

	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1    4   
			//2	   3 5
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iCurIndex += 6;
		}
	}

	GenVertexNormal();
	return true;
}
bool TMap::GenVertexNormal()
{
	///////////////////////////////////////////////////////////////////
	//// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	///////////////////////////////////////////////////////////////////
	InitFaceNormals();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	return true;
}
bool TMap::UpdateBuffer()
{
	///////////////////////////////////////////////////////////////////
	//// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	///////////////////////////////////////////////////////////////////
	//InitFaceNormals();
	//GenNormalLookupTable();
	//ReLoadVBuffer();
	return true;
}
bool TMap::CreateMap(TMapDesc& MapDesc)
{
	// 대용량의 사이즈를 저정한다.( pow(2.0f, 10.0f) + 1 이상)
	if (MapDesc.iNumCols > 1025 || MapDesc.iNumRows > 1025)
	{
		MapDesc.iNumCols = 1025; // 정점개수 105만개,
		MapDesc.iNumRows = 1025; // 페이스 개수 209만개 
	}
	m_MapDesc = MapDesc;
	m_iNumRows = MapDesc.iNumCols;
	m_iNumCols = MapDesc.iNumRows;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumVertices = m_iNumRows*m_iNumCols;
	m_iNumFace = m_iNumCellRows*m_iNumCellCols * 2;
	m_fCellDistance = MapDesc.fCellDistance;
	return true;
}
float TMap::GetHeightmap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y * m_MapDesc.fScaleHeight;
}
float TMap::GetHeightOfVertex(UINT Index)
{
	return 0.0f;
};
TVector3 TMap::GetNormalOfVertex(UINT Index)
{
	return TVector3(0.0f, 1.0f, 0.0f);
};
TVector4 TMap::GetColorOfVertex(UINT Index)
{
	return TVector4(1, 1, 1, 1.0f);
};
TVector2 TMap::GetTextureOfVertex(float fOffsetX, float fOffsetY)
{
	return TVector2(fOffsetX, fOffsetY);
};
void TMap::CalcVertexColor(TVector3 vLightDir)
{
	/////////////////////////////////////////////////////////////////
	// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	/////////////////////////////////////////////////////////////////	
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			//<vLightDir = 0,-1.0f,0>
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			//D3DXVec3Normalize(	&m_VertexList[iVertexIndex].n, 
			//					&m_VertexList[iVertexIndex].n);
			float fDot =  
			D3DXVec3Dot(&-vLightDir,&m_VertexList[iVertexIndex].n);
			m_VertexList[iVertexIndex].c *= fDot;
			m_VertexList[iVertexIndex].c.w = 1.0f;
		}
	}
}
bool TMap::ReLoadVBuffer()
{
	CalcPerVertexNormalsFastLookup();
	m_pContext->UpdateSubresource(m_dxobj.g_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}
bool TMap::ReLoadIBuffer()
{
	m_pContext->UpdateSubresource(m_dxobj.g_pIndexBuffer.Get(), 0, NULL, &m_IndexList.at(0), 0, 0);
	return true;
}
void	TMap::UpdateIndexBuffer(ID3D11DeviceContext*  pContext, DWORD* pdwIndexArray, int iFaceCount)
{
	assert(pdwIndexArray);
	m_iNumFace = iFaceCount;
	m_dxobj.m_iNumIndex = m_iNumFace * 3;
	pContext->UpdateSubresource(m_dxobj.g_pIndexBuffer.Get(), 0, NULL, pdwIndexArray, 0, 0);
}
bool TMap::Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext*  pContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	return true;
}
bool TMap::Load(TMapDesc& MapDesc)
{
	if (!CreateMap(MapDesc))
	{
		return false;
	}

	if (!TActor::Create(m_pd3dDevice, MapDesc.strShaderFile.c_str(), MapDesc.strTextureFile.c_str()))
	{
		return false;
	}

	return true;
}

bool TMap::Render(ID3D11DeviceContext*  pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool TMap::Frame()
{
	return true;
}
bool TMap::Release()
{
	if (m_pNormalLookupTable)
	{
		free(m_pNormalLookupTable);
		m_pNormalLookupTable = NULL;
	}
	if (m_pFaceNormals != NULL)
	{
		delete m_pFaceNormals;
		m_pFaceNormals = NULL;
	}
	return true;
}
void TMap::InitFaceNormals()
{
	m_pFaceNormals = new TVector3[m_iNumFace];

	for (int i = 0; i<m_iNumFace; i++)
	{
		m_pFaceNormals[i] = TVector3(0.0f, 0.0f, 0.0f);
	}
}
//--------------------------------------------------------------------------------------
// Create face normals for the mesh
//--------------------------------------------------------------------------------------
void TMap::CalcFaceNormals()
{
	// Loop over how many faces there are
	int j = 0;
	for (int i = 0; i< m_iNumFace * 3; i += 3)
	{
		DWORD i0 = m_IndexList[i];
		DWORD i1 = m_IndexList[i + 1];
		DWORD i2 = m_IndexList[i + 2];
		m_pFaceNormals[j] = ComputeFaceNormal(i0, i1, i2);
		j++;
	}
}
//--------------------------------------------------------------------------------------
// Create a fast normal lookup table
//--------------------------------------------------------------------------------------
void TMap::GenNormalLookupTable()
{
	// We're going to create a table that lists, for each vertex, the
	// triangles which that vertex is a part of.

	if (m_pNormalLookupTable != NULL)
	{
		free(m_pNormalLookupTable);
		m_pNormalLookupTable = NULL;
	}

	// Each vertex may be a part of up to 6 triangles in the grid, so
	// create a buffer to hold a pointer to the normal of each neighbor.
	int buffersize = m_iNumRows*m_iNumCols * 6;

	m_pNormalLookupTable = (int *)malloc(sizeof(void *) * buffersize);
	for (int i = 0; i<buffersize; i++)
		m_pNormalLookupTable[i] = -1;

	// Now that the table is initialized, populate it with the triangle data.

	// For each triangle
	//   For each vertex in that triangle
	//     Append the triangle number to lookuptable[vertex]
	for (int i = 0; i < m_iNumFace; i++)
	{
		for (int j = 0; j<3; j++)
		{
			// Find the next empty slot in the vertex's lookup table "slot"
			for (int k = 0; k<6; k++)
			{
				int vertex = m_IndexList[i * 3 + j];
				if (m_pNormalLookupTable[vertex * 6 + k] == -1)
				{
					m_pNormalLookupTable[vertex * 6 + k] = i;
					break;
				}
			}
		}  // For each vertex that is part of the current triangle
	} // For each triangle
}

//--------------------------------------------------------------------------------------
// Compute vertex normals from the fast normal lookup table
//--------------------------------------------------------------------------------------
void TMap::CalcPerVertexNormalsFastLookup()
{
	// First, calculate the face normals for each triangle.
	CalcFaceNormals();

	// For each vertex, sum the normals indexed by the normal lookup table.
	int j = 0;
	for (int i = 0; i < m_iNumVertices; i++)
	{
		TVector3 avgNormal;
		avgNormal = TVector3(0.0f, 0.0f, 0.0f);

		// Find all the triangles that this vertex is a part of.
		for (j = 0; j<6; j++)
		{
			int triIndex;
			triIndex = m_pNormalLookupTable[i * 6 + j];

			// If the triangle index is valid, get the normal and average it in.
			if (triIndex != -1)
			{
				avgNormal += m_pFaceNormals[triIndex];
			}
			else
				break;
		}

		// Complete the averaging step by dividing & normalizing.
		_ASSERT(j > 0);
		avgNormal.x /= (float)j;//.DivConst( (float)(j) );
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		D3DXVec3Normalize(&avgNormal, &avgNormal);

		// Set the vertex normal to this new normal.
		m_VertexList[i].n.x = avgNormal.x;
		m_VertexList[i].n.y = avgNormal.y;
		m_VertexList[i].n.z = avgNormal.z;

	}  // For each vertex

	/////////////////////////////////////////////////////////////////
	// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	/////////////////////////////////////////////////////////////////	
	if( m_bStaticLight) CalcVertexColor(m_vLightDir);
}

TMap::TMap(void)
{
	m_iNumFace = 0;
	m_iDiffuseTex = 0;
	m_iNumCols = 0;
	m_iNumRows = 0;
	m_pd3dDevice = nullptr;
	m_pNormalLookupTable = nullptr;
	m_pFaceNormals = nullptr;
	m_vLightDir = TVector3(0, -1, 0);
	m_bStaticLight = false;
}

TMap::~TMap(void)
{
}
