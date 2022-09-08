#include "TObjectPlane.h"

void    TObjectPlane::CreateVertexData()
{
    m_VertexList.resize(4);
    m_VertexList[0].p = { -0.5f, 0.5f, 0.0f };
    m_VertexList[1].p = { +0.5f, 0.5f,  0.0f };
    m_VertexList[2].p = { -0.5f, -0.5f, 0.0f };
    m_VertexList[3].p = { 0.5f, -0.5f, 0.0f };

    m_VertexList[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_VertexList[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_VertexList[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_VertexList[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_VertexList[0].t = { 0.0f, 0.0f };
    m_VertexList[1].t = { 1.0f, 0.0f };
    m_VertexList[2].t = { 0.0f, 1.0f };
    m_VertexList[3].t = { 1.0f, 1.0f };
}

void   TObjectPlane::UpdateVertexBuffer()
{
    m_VertexList[0].p = { -0.1f, 0.1f, 0.0f };
    m_VertexList[1].p = { +0.1f, 0.1f,  0.0f };
    m_VertexList[2].p = { -0.1f, -0.1f, 0.0f };
    m_VertexList[3].p = { 0.1f, -0.1f, 0.0f };

    TBaseObject::UpdateVertexBuffer();
}