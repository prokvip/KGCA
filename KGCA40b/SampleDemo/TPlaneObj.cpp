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
	m_VertexList[0].p = T::TVector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[0].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[0].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[0].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[1].p = T::TVector3(1.0f, 1.0f, 0.0f);
	m_VertexList[1].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[1].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[1].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[2].p = T::TVector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[2].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[2].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[2].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[3].p = T::TVector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[3].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[3].t = T::TVector2(1.0f, 1.0f);
	return true;
}
bool		TPlaneObj::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(3); m_IndexList.push_back(2); m_IndexList.push_back(1);
	return true;
}
TPlaneObj::TPlaneObj()
{

}
TPlaneObj::~TPlaneObj()
{

}