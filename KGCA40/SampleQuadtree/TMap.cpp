#include "TMap.h"
bool    TMap::Load(TMapInfo& info)
{
    m_info = info;
    m_info.m_iNumColCell = m_info.m_iNumCol - 1;
    m_info.m_iNumRowCell = m_info.m_iNumRow - 1;
    m_info.m_iNumVertex = m_info.m_iNumCol * m_info.m_iNumRow;
    return true;
}
bool	TMap::CreateVertexData()
{
    //m_VertexList
    // 0  1  2
    // 3  4  5
    // 6  7  8

    m_VertexList.resize(m_info.m_iNumVertex);

    int iIndex = 0;
    for (int iRow=0; iRow < m_info.m_iNumRow; iRow++)
    {
        for (int iCol = 0; iCol < m_info.m_iNumCol; iCol++)
        {
            int iIndex = iRow * m_info.m_iNumRow + iCol;
            m_VertexList[iIndex].pos.x = m_info.m_fCellDistance * iCol;
            m_VertexList[iIndex].pos.y = 0.0f;
            m_VertexList[iIndex].pos.z = -m_info.m_fCellDistance * iRow;
            m_VertexList[iIndex].color = 
                TVector4(
                    randstep(0.0f, 1.0f),
                    randstep(0.0f, 1.0f),
                    randstep(0.0f, 1.0f),1.0f);            
        }
    }
    if (m_VertexList.size() > 0) return true;
    return false;
}
bool	TMap::CreateIndexData()
{
    //m_VertexList
    // 0    1   2
    // 
    // 3    4   5 

    // 6    7  8

    m_IndexList.resize(m_info.m_iNumColCell * m_info.m_iNumRowCell * 2 * 3);

    int iIndex = 0;
    for (int iRow = 0; iRow < m_info.m_iNumRowCell; iRow++)
    {
        for (int iCol = 0; iCol < m_info.m_iNumColCell; iCol++)
        {
            int iCurrentIndex = iRow * m_info.m_iNumRow + iCol;
            int iNextRow = (iRow+1) * m_info.m_iNumRow + iCol;
            m_IndexList[iIndex+0] = iCurrentIndex;
            m_IndexList[iIndex+1] = iCurrentIndex+1;
            m_IndexList[iIndex+2] = iNextRow;

            m_IndexList[iIndex+3] = m_IndexList[iIndex + 2];
            m_IndexList[iIndex+4] = m_IndexList[iIndex + 1];
            m_IndexList[iIndex+5] = iNextRow + 1;
            iIndex += 6;
        }
    }
    if (m_IndexList.size() > 0) return true;
    return false;
}
