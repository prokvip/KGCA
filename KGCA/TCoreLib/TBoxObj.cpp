#include "TBoxObj.h"
bool		TBoxObj::SetVertexData()
{
	// Local Coord
	// Box
	//  0        1 
	//     ¿øÁ¡
	//  2        3
	m_VertexList.resize(24);

	int index = 0;
	// +Z plane
	m_VertexList[index].p = T::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);
	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);
	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);
	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);
	// -Z plane
	
	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = T::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	
	// +X plane
	m_VertexList[++index].p = T::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = T::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// -X plane
	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// -y plane
	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = T::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = T::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// +y plane
	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = T::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = T::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = T::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	GenAABB();
	return true;
}
bool		TBoxObj::SetIndexData()
{
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	m_IndexList.push_back(4); m_IndexList.push_back(5); m_IndexList.push_back(6);
	m_IndexList.push_back(6); m_IndexList.push_back(5); m_IndexList.push_back(7);
	m_IndexList.push_back(8); m_IndexList.push_back(9); m_IndexList.push_back(10);
	m_IndexList.push_back(10); m_IndexList.push_back(9); m_IndexList.push_back(11);
	m_IndexList.push_back(12); m_IndexList.push_back(13); m_IndexList.push_back(14);
	m_IndexList.push_back(14); m_IndexList.push_back(13); m_IndexList.push_back(15);
	m_IndexList.push_back(16); m_IndexList.push_back(17); m_IndexList.push_back(18);
	m_IndexList.push_back(18); m_IndexList.push_back(17); m_IndexList.push_back(19);
	m_IndexList.push_back(20); m_IndexList.push_back(21); m_IndexList.push_back(22);
	m_IndexList.push_back(22); m_IndexList.push_back(21); m_IndexList.push_back(23);

	return true;
}
bool	TBoxObj::PostRender()
{
	if (m_IndexList.size() <= 0)
		m_pContext->Draw(m_VertexList.size(), 0);
	else
	{
		m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	return true;
}
void TBoxObj::DrawDebugRender(TBox* pBox)
{
	//pBox->vList[0] = T::TVector3(pBox->vMin.x,
	//	pBox->vMax.y,
	//	pBox->vMin.z);
	//pBox->vList[1] = T::TVector3(pBox->vMax.x,
	//	pBox->vMax.y,
	//	pBox->vMin.z);
	//pBox->vList[2] = T::TVector3(pBox->vMin.x,
	//	pBox->vMin.y,
	//	pBox->vMin.z);
	//pBox->vList[3] = T::TVector3(pBox->vMax.x,
	//	pBox->vMin.y,
	//	pBox->vMin.z);

	//pBox->vList[4] = T::TVector3(pBox->vMin.x,
	//	pBox->vMax.y,
	//	pBox->vMax.z);
	//pBox->vList[5] = T::TVector3(pBox->vMax.x,
	//	pBox->vMax.y,
	//	pBox->vMax.z);
	//pBox->vList[6] = T::TVector3(pBox->vMin.x,
	//	pBox->vMin.y,
	//	pBox->vMax.z);
	//pBox->vList[7] = T::TVector3(pBox->vMax.x,
	//	pBox->vMin.y,
	//	pBox->vMax.z);
	// 4      5
	// 6      7

	// 0     1
	// 2     3
	int index = 0;
	// +z

	m_VertexList[index].p = pBox->vList[5];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[4];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[7];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[6];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);
	
	m_VertexList[++index].p = pBox->vList[0];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[1];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[2];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[3];
	m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	
	m_VertexList[++index].p = pBox->vList[1];
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[5];
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[3];
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[7];
	m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	
	m_VertexList[++index].p = pBox->vList[4];
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[0];
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[6];
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[2];
	m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	
	m_VertexList[++index].p = pBox->vList[2];
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[3];
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[6];
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[7];
	m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);
	
	m_VertexList[++index].p = pBox->vList[4];
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[5];
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_VertexList[++index].p = pBox->vList[0];
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_VertexList[++index].p = pBox->vList[1];
	m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	//SetMatrix(&m_matWorld, &matView, &matProj);
	PreRender();
	Draw();
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	PostRender();
}
TBoxObj::TBoxObj()
{

}
TBoxObj::~TBoxObj()
{

}