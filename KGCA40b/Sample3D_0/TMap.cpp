#include "TMap.h"
bool		TMap::CreateMap(UINT width, UINT height,
	float fDistance)
{
	m_iNumCols = width;
	m_iNumRows = height;
	m_iCellDistance = fDistance;
	m_iNumVertices = m_iNumCols * m_iNumRows;
	m_iNumCellCols = m_iNumCols-1;
	m_iNumCellRows = m_iNumRows-1;
	m_iNumFaces = m_iNumCellCols* m_iNumCellRows*2;

	return true;
}
bool		TMap::SetVertexData()
{
	m_VertexList.resize(m_iNumVertices);	
	float  hHalfCol = (m_iNumCols - 1) / 2.0f;
	float  hHalfRow = (m_iNumRows - 1) / 2.0f;
	float  ftxOffetU = 1.0f / (m_iNumCols - 1);
	float  ftxOffetV = 1.0f / (m_iNumRows - 1);
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int index = iRow * m_iNumCols + iCol;
			m_VertexList[index].p.x = (iCol- hHalfCol)* m_iCellDistance;
			m_VertexList[index].p.y = 0.0f;
			m_VertexList[index].p.z = -((iRow - hHalfRow)* m_iCellDistance);
			m_VertexList[index].n = TVector3(0, 1, 0);
			m_VertexList[index].c = TVector4(1, 1, 1,1);
			m_VertexList[index].t = 
				TVector2(ftxOffetU*iCol, ftxOffetV * iRow);
		}
	}
	return true;
}
bool		TMap::SetIndexData()
{
	m_IndexList.resize(m_iNumFaces*3);
	UINT iIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			m_IndexList[iIndex+0] = iRow * m_iNumCols + iCol;
			m_IndexList[iIndex+1] = (iRow * m_iNumCols + iCol)+1;
			m_IndexList[iIndex+2] = (iRow+1)* m_iNumCols + iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 2]+1;
			
			iIndex += 6;
		}
	}
	return true;
}
TMap::TMap()
{

}
TMap::~TMap()
{

}