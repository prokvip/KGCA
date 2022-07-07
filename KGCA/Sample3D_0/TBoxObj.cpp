#include "TBoxObj.h"
bool		TBoxObj::SetVertexData()
{
	// Local Coord
	// Box
	//  0        1 
	//     ����
	//  2        3
	m_VertexList.resize(24);

	int index = 0;
	// +Z plane
	m_VertexList[index].p = TMath::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);
	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);
	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);
	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, 1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);
	// -Z plane
	
	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = TMath::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 0.0f, -1.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);

	
	// +X plane
	m_VertexList[++index].p = TMath::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = TMath::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);

	// -X plane
	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(-1.0f, 0.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);

	// -y plane
	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = TMath::TVector3(-1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = TMath::TVector3(1.0f, -1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, -1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);

	// +y plane
	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 0.0f);

	m_VertexList[++index].p = TMath::TVector3(1.0f, 1.0f, 1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 0.0f);


	m_VertexList[++index].p = TMath::TVector3(-1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(0.0f, 1.0f);


	m_VertexList[++index].p = TMath::TVector3(1.0f, 1.0f, -1.0f);
	m_VertexList[index].n = TMath::TVector3(0.0f, 1.0f, 0.0f);
	m_VertexList[index].c = TMath::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[index].t = TMath::TVector2(1.0f, 1.0f);

	m_pTexCube = I_Texture.Load(L"..\\..\\data\\sky\\LobbyCube.dds");

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
		m_pContext->PSSetShaderResources(3, 1,
			m_pTexCube->m_pSRV.GetAddressOf());
		m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	return true;
}
TBoxObj::TBoxObj()
{

}
TBoxObj::~TBoxObj()
{

}