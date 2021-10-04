#include "Sample.h"
#include "TVector.h"
#include "SimpleMath.h"
TBASIS_RUN(SampleLibrary)
Sample::Sample()
{
    m_pSpeed = 3.0f;
    m_vCameraPos =    { 8, 20, -20.0f };
    m_vCameraTarget = { 8, 0, -19.0f };
}
bool Sample::Init()
{
   

    TMapInfo info{
            64 + 1, 
            64 + 1, 0,0, 0,     
            1.0f
    };
    if (m_Map.Load(info))
    {
        m_Map.Init();
    }

    m_vCameraPos = { m_Map.m_info.m_iNumCol/2.0f, m_Map.m_info.m_iNumCol / 2.0f, -(m_Map.m_info.m_iNumRow / 2.0f) };
    m_vCameraTarget = { m_Map.m_info.m_iNumCol / 2.0f, 0, -(m_Map.m_info.m_iNumRow / 2.0f)+1.0f };

    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].LoadObject(L"ObjectData.txt");
        m_BoxObj[iObj].Init();
    }
    return true;
}
bool Sample::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('R') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_pSpeed * g_fSecPerFrame;
    }
    // -1 ~ +1 -> 0 ~ 1 => -1*0.5f+0.5f;
    // 0 ~ +1 -> -1 ~ 1 => 0.5f*2.0f-1.0f;
    // D3D11_USAGE_DEFAULT
    //m_cbData.matWorld._11 = cosf(g_fGameTimer)*0.5f+0.5f;
    //m_cbData.matWorld = TMatrix::RotationY(g_fGameTimer);
    //DirectX::SimpleMath::Matrix  mat;
    //mat = mat.CreateRotationY(g_fGameTimer);
    //memcpy( &m_cbData.matWorld,    &mat, sizeof(TMatrix));

    TVector3 vUp = { 0,1,0.0f };
    m_cbData.matView = TMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
    m_cbData.matProj = TMatrix::PerspectiveFovLH(1.0f, 
        1000.0f, TBASIS_PI * 0.5f, 
        (float)g_rtClient.right / (float)g_rtClient.bottom);

    m_Map.Frame();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Frame();
    }
    return true;
}

bool Sample::Render()
{
    m_Map.SetMatrix(
        &m_cbData.matWorld,
        &m_cbData.matView,
        &m_cbData.matProj);
    m_Map.Render(m_pImmediateContext);

    m_BoxObj[0].m_matWorld._41= -3.0f;
    m_BoxObj[1].m_matWorld._41 = 3.0f;

    for (int iObj = 0; iObj < 2; iObj++)
    {        
        m_BoxObj[iObj].SetMatrix(
            &m_BoxObj[iObj].m_matWorld,
            &m_cbData.matView,
            &m_cbData.matProj);
        m_BoxObj[iObj].Render(m_pImmediateContext);
    }
   
    return false;
}

bool Sample::Release()
{
    m_Map.Release();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Release();
    }
    return false;
}
