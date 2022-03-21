#include "TPlaneObj.h"
bool		TPlaneObj::SetVertexData()
{
	// Local Coord
	// Box
	//  0        1 
	//     ¿øÁ¡
	//  2        3
	m_VertexList.resize(4);
	// -Z plane
	m_VertexList[0].p = TVector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[0].n = TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[0].t = TVector2(0.0f, 0.0f);

	m_VertexList[1].p = TVector3(1.0f, 1.0f, 0.0f);
	m_VertexList[1].n = TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[1].t = TVector2(1.0f, 0.0f);


	m_VertexList[2].p = TVector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[2].n = TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[2].t = TVector2(0.0f, 1.0f);


	m_VertexList[3].p = TVector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].n = TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[3].t = TVector2(1.0f, 1.0f);
	return true;
}
bool		TPlaneObj::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(3); m_IndexList.push_back(1);
	return true;
}
TPlaneObj::TPlaneObj()
{

}
TPlaneObj::~TPlaneObj()
{

}