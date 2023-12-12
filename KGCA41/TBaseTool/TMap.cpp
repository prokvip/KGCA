#include "pch.h"
#include "TMap.h"
bool TMap::CreateVertexData() 
{
	m_VertexList.resize(m_MapInfo.col * m_MapInfo.row);
	float fHalfCols = (m_MapInfo.col -1)* 0.5f;
	float fHalfRows = (m_MapInfo.row - 1) * 0.5f;
	m_iNumCols = m_MapInfo.col;
	m_iNumRows = m_MapInfo.row;
	m_iNumCellRows = (m_MapInfo.row - 1);
	m_iNumCellCols = (m_MapInfo.col - 1);
	m_iNumFace = m_iNumCellRows * m_iNumCellCols * 2;

	for (int iRow = 0; iRow < m_MapInfo.row; iRow++)
	{
		for (int iCol = 0; iCol < m_MapInfo.col; iCol++)
		{
			int  iVertexIndex = iRow * m_MapInfo.col + iCol;
			m_VertexList[iVertexIndex].p.x = (iCol - fHalfCols) * m_MapInfo.fDistance;
			m_VertexList[iVertexIndex].p.z = -((iRow - fHalfRows) * m_MapInfo.fDistance);
			m_VertexList[iVertexIndex].p.y = GetHeightOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].n = GetNormalOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].c = GetColorOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].t = 
				GetTextureOfVertex(iRow, iCol);
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
	return true;
};
bool   TMap::Load(TMapInfo info)
{
	m_MapInfo = info;
	TObject::Create(
		info.strDefaultTextureFileName, info.strShaderFileName );
	return true;
}

float		TMap::GetHeightOfVertex(UINT iVertexIndex)
{
	return 0.0f;
}
TVector3	TMap::GetNormalOfVertex(UINT iVertexIndex)
{
	return TVector3(0,1,0);
}
TVector4	TMap::GetColorOfVertex(UINT iVertexIndex)
{
	return TVector4(1,1, 1, 1);
}
TVector2	TMap::GetTextureOfVertex(UINT row, UINT col)
{
	return TVector2((float)col / (float)(m_iNumCols - 1),
		(float)row / (float)(m_iNumRows - 1));
}