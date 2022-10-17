#include "TShape.h"
#include "TInput.h"
void    TShapeBox::CreateVertexData()
{
	// »ó´Ü
	  // 5    6
	  // 1    2
	  // ÇÏ´Ü
	  // 4    7
	  // 0    3  
	  // ¾Õ¸é
	m_VertexList.resize(24);
	m_VertexList[0] = SimpleVertex(TVector(-1.0f, 1.0f, .0f),  TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(TVector(1.0f, 1.0f, .0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[2] = SimpleVertex(TVector(1.0f, -1.0f, .0f),  TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[3] = SimpleVertex(TVector(-1.0f, -1.0f, .0f),  TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = SimpleVertex(TVector(1.0f, 1.0f, 1.0f),  TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(TVector(-1.0f, 1.0f, 1.0f),  TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[6] = SimpleVertex(TVector(-1.0f, -1.0f, 1.0f),  TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[7] = SimpleVertex(TVector(1.0f, -1.0f, 1.0f),  TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = SimpleVertex(TVector(1.0f, 1.0f, -1.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[9] = SimpleVertex(TVector(1.0f, 1.0f, 1.0f),  TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[10] = SimpleVertex(TVector(1.0f, -1.0f, 1.0f),  TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[11] = SimpleVertex(TVector(1.0f, -1.0f, -1.0f),  TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = SimpleVertex(TVector(-1.0f, 1.0f, 1.0f),  TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[13] = SimpleVertex(TVector(-1.0f, 1.0f, -1.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[14] = SimpleVertex(TVector(-1.0f, -1.0f, -1.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[15] = SimpleVertex(TVector(-1.0f, -1.0f, 1.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = SimpleVertex(TVector(-1.0f, 1.0f, 1.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[17] = SimpleVertex(TVector(1.0f, 1.0f, 1.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[18] = SimpleVertex(TVector(1.0f, 1.0f, -1.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[19] = SimpleVertex(TVector(-1.0f, 1.0f, -1.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = SimpleVertex(TVector(-1.0f, -1.0f, -1.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[21] = SimpleVertex(TVector(1.0f, -1.0f, -1.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[22] = SimpleVertex(TVector(1.0f, -1.0f, 1.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[23] = SimpleVertex(TVector(-1.0f, -1.0f, 1.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));


    m_InitVertexList = m_VertexList;
}
void    TShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

}

bool TShapeBox::Render()
{
    PreRender();
    PostRender();
    return true;
}
bool TShapeBox::Frame()
{
	//camera
	TMatrix matView;
	TVector vPos = { 0,5,-5 };
	static TVector vPosMovement = { 0,0,0 };
	TVector vTarget = { 0,0,0 };
	if (I_Input.GetKey('W') == KEY_HOLD)
	{
		vPosMovement.z += 10.0f * g_fSecondPerFrame;
		//vTarget.z += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('S') == KEY_HOLD)
	{
		vPosMovement.z -= 10.0f * g_fSecondPerFrame;
		//vTarget.z -= 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		vPosMovement.x -= 10.0f * g_fSecondPerFrame;
		//vTarget.x -= 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		vPosMovement.x += 10.0f * g_fSecondPerFrame;
		//vTarget.x += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('Q') == KEY_HOLD)
	{
		vPosMovement.y += 10.0f * g_fSecondPerFrame;
		//vTarget.y += 10.0f * g_fSecondPerFrame;
	}
	if (I_Input.GetKey('E') == KEY_HOLD)
	{
		vPosMovement.y -= 10.0f * g_fSecondPerFrame;
		//vTarget.y -= 10.0f * g_fSecondPerFrame;
	}

	TVector vUp = { 0,1,0 };
	TMatrix mCamera = TMath::RotationY(g_fGameTimer);
	vPos = (vPos+ vPosMovement) * mCamera;
	matView.ViewLookAt(vPos, vTarget, vUp);

	TMatrix matProj;
	matProj.PerspectiveFovLH(1.0f, 100.0f, 3.141592f * 0.5f, 800.0f / 600.0f);
	//matProj.OrthoLH(800, 600, 0.0f, 100.0f);
	OrthoOffCenterLH(matProj ,-400, 400, -300, 300, 0.0f, 100.0f);
	TMatrix m, s, t, c;
	float fScale = cos(g_fGameTimer) * 0.5f + 0.5f;
	//s = TMath::Scale(400, 300, 10);
	//m = TMath::RotationZ(g_fGameTimer);
	//t = TMath::Translation(0.0f, 0, 0);
	c = s * m * t;

	m_cbData.matWorld = c;
	m_cbData.matView = matView;
	m_cbData.matProj = matProj;
	m_cbData.fTimer = g_fGameTimer;
	UpdateConstantBuffer();
	return true;
}