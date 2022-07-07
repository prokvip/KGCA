#include "TSkyObj.h"
bool		TSkyObj::LoadTexture(const TCHAR* szColorFileName,
								 const TCHAR* szMaskFileName)
{

	m_pTexArray[0] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_front.bmp" );
	m_pTexArray[1] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_back.bmp" );
	m_pTexArray[2] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_right.bmp" );
	m_pTexArray[3] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_left.bmp" );
	m_pTexArray[4] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_up.bmp" );
	m_pTexArray[5] = I_Texture.Load(L"..\\..\\data\\sky\\st00_cm_down.bmp" );
	
	m_pSRVArray[0] = m_pTexArray[0]->m_pSRV;
	m_pSRVArray[1] = m_pTexArray[1]->m_pSRV;
	m_pSRVArray[2] = m_pTexArray[2]->m_pSRV;
	m_pSRVArray[3] = m_pTexArray[3]->m_pSRV;
	m_pSRVArray[4] = m_pTexArray[4]->m_pSRV;
	m_pSRVArray[5] = m_pTexArray[5]->m_pSRV;

	m_pTexCube = I_Texture.Load(L"..\\..\\data\\sky\\LobbyCube.dds");

	m_TextureDesc = m_pTexArray[0]->m_TextureDesc;
	return true;
}
bool		TSkyObj::SetVertexData()
{
	// Local Coord
	// Box
	//  0        1 
	//     ¿øÁ¡
	//  2        3
	m_VertexList.resize(24);

	int index = 0;
	// »ó´Ü
	// 5    6
	// 1    2
	// ÇÏ´Ü
	// 4    7
	// 0    3  
	// ¾Õ¸é
	m_VertexList.resize(24);
	m_VertexList[0] = TVertex(T::TVector3(-1.0f, 1.0f, -1.0f), T::TVector3(0.0f, 0.0f, -1.0f), T::TVector4(1.0f, 0.0f, 0.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[1] = TVertex(T::TVector3(1.0f, 1.0f, -1.0f), T::TVector3(0.0f, 0.0f, -1.0f), T::TVector4(1.0f, 0.0f, 0.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[2] = TVertex(T::TVector3(1.0f, -1.0f, -1.0f), T::TVector3(0.0f, 0.0f, -1.0f), T::TVector4(1.0f, 0.0f, 0.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[3] = TVertex(T::TVector3(-1.0f, -1.0f, -1.0f), T::TVector3(0.0f, 0.0f, -1.0f), T::TVector4(1.0f, 0.0f, 0.0f, 1.0f), T::TVector2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = TVertex(T::TVector3(1.0f, 1.0f, 1.0f), T::TVector3(0.0f, 0.0f, 1.0f), T::TVector4(0.0f, 0.0f, 0.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[5] = TVertex(T::TVector3(-1.0f, 1.0f, 1.0f), T::TVector3(0.0f, 0.0f, 1.0f), T::TVector4(0.0f, 1.0f, 0.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[6] = TVertex(T::TVector3(-1.0f, -1.0f, 1.0f), T::TVector3(0.0f, 0.0f, 1.0f), T::TVector4(0.0f, 1.0f, 0.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[7] = TVertex(T::TVector3(1.0f, -1.0f, 1.0f), T::TVector3(0.0f, 0.0f, 1.0f), T::TVector4(0.0f, 1.0f, 0.0f, 1.0f), T::TVector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = TVertex(T::TVector3(1.0f, 1.0f, -1.0f), T::TVector3(1.0f, 0.0f, 0.0f), T::TVector4(0.0f, 0.0f, 1.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[9] = TVertex(T::TVector3(1.0f, 1.0f, 1.0f), T::TVector3(1.0f, 0.0f, 0.0f), T::TVector4(0.0f, 0.0f, 1.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[10] = TVertex(T::TVector3(1.0f, -1.0f, 1.0f), T::TVector3(1.0f, 0.0f, 0.0f), T::TVector4(0.0f, 0.0f, 1.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[11] = TVertex(T::TVector3(1.0f, -1.0f, -1.0f), T::TVector3(1.0f, 0.0f, 0.0f), T::TVector4(0.0f, 0.0f, 1.0f, 1.0f), T::TVector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = TVertex(T::TVector3(-1.0f, 1.0f, 1.0f), T::TVector3(-1.0f, 0.0f, 0.0f), T::TVector4(1.0f, 1.0f, 0.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[13] = TVertex(T::TVector3(-1.0f, 1.0f, -1.0f), T::TVector3(-1.0f, 0.0f, 0.0f), T::TVector4(1.0f, 1.0f, 0.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[14] = TVertex(T::TVector3(-1.0f, -1.0f, -1.0f), T::TVector3(-1.0f, 0.0f, 0.0f), T::TVector4(1.0f, 1.0f, 0.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[15] = TVertex(T::TVector3(-1.0f, -1.0f, 1.0f), T::TVector3(-1.0f, 0.0f, 0.0f), T::TVector4(1.0f, 1.0f, 0.0f, 1.0f), T::TVector2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = TVertex(T::TVector3(-1.0f, 1.0f, 1.0f), T::TVector3(0.0f, 1.0f, 0.0f), T::TVector4(1.0f, 0.5f, 1.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[17] = TVertex(T::TVector3(1.0f, 1.0f, 1.0f), T::TVector3(0.0f, 1.0f, 0.0f), T::TVector4(1.0f, 0.5f, 1.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[18] = TVertex(T::TVector3(1.0f, 1.0f, -1.0f), T::TVector3(0.0f, 1.0f, 0.0f), T::TVector4(1.0f, 0.5f, 1.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[19] = TVertex(T::TVector3(-1.0f, 1.0f, -1.0f), T::TVector3(0.0f, 1.0f, 0.0f), T::TVector4(1.0f, 0.5f, 1.0f, 1.0f), T::TVector2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = TVertex(T::TVector3(-1.0f, -1.0f, -1.0f), T::TVector3(0.0f, -1.0f, 0.0f), T::TVector4(0.0f, 1.0f, 1.0f, 1.0f), T::TVector2(0.0f, 0.0f));
	m_VertexList[21] = TVertex(T::TVector3(1.0f, -1.0f, -1.0f), T::TVector3(0.0f, -1.0f, 0.0f), T::TVector4(0.0f, 1.0f, 1.0f, 1.0f), T::TVector2(1.0f, 0.0f));
	m_VertexList[22] = TVertex(T::TVector3(1.0f, -1.0f, 1.0f), T::TVector3(0.0f, -1.0f, 0.0f), T::TVector4(0.0f, 1.0f, 1.0f, 1.0f), T::TVector2(1.0f, 1.0f));
	m_VertexList[23] = TVertex(T::TVector3(-1.0f, -1.0f, 1.0f), T::TVector3(0.0f, -1.0f, 0.0f), T::TVector4(0.0f, 1.0f, 1.0f, 1.0f), T::TVector2(0.0f, 1.0f));

	GenAABB();
	return true;
}
bool		TSkyObj::SetIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	return true;
}
bool	TSkyObj::PostRender()
{
	//// 1¹ø
	//for (int iTex = 0; iTex < 6; iTex++)
	//{
	//	m_pContext->PSSetShaderResources(0, 1,
	//		m_pTexArray[iTex]->m_pSRV.GetAddressOf());
	//	m_pContext->DrawIndexed(6, 6*iTex, 0);
	//}
	// 1¹ø
	/*m_pContext->PSSetShaderResources(2, 6,
		m_pSRVArray[0].GetAddressOf());
	m_pContext->DrawIndexed(36, 0, 0);*/
	// 2¹ø
	m_pContext->PSSetShaderResources(3, 1,
		m_pTexCube->m_pSRV.GetAddressOf());
	m_pContext->DrawIndexed(36, 0, 0); 
	return true;
}
TSkyObj::TSkyObj()
{

}
TSkyObj::~TSkyObj()
{

}