#include "Sample.h"
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool Sample::Init()
{
    TMapInfo info{
            4 + 1, 
            4 + 1, 0,0, 0,
            1.0f
    };
    if (m_Map.Load(info))
    {
        m_Map.Init();
    }
    m_Quadtree.Build(&m_Map);
    
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].LoadObject(L"ObjectData.txt");
        m_BoxObj[iObj].Init();
    }*/

    m_Camera.Init();
    m_Camera.CreateViewMatrix(  TVector3(16,3,-31), 
                                TVector3(16,3,100000));
    m_Camera.CreateProjMatrix(1.0f,
        1000.0f, TBASIS_PI * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);
    return true;
}
bool Sample::Frame()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    m_fYaw += g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    m_fPitch += g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.Update(TVector4(m_fPitch, m_fYaw, 0.0f, 0.0f));
    m_Camera.Frame();

    m_Map.Frame();
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Frame();
    }*/
    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    return true;
}

bool Sample::Render()
{
    m_Map.SetMatrix(
        nullptr,
        &m_Camera.m_matView,
        &m_Camera.m_matProj);
    m_Map.Render(m_pImmediateContext);

  /*  m_BoxObj[0].m_matWorld._41= -3.0f;
    m_BoxObj[1].m_matWorld._41 = 3.0f;

    for (int iObj = 0; iObj < 2; iObj++)
    {        
        m_BoxObj[iObj].SetMatrix(
            &m_BoxObj[iObj].m_matWorld,
            &m_Camera.m_matView,
            &m_Camera.m_matProj);
        m_BoxObj[iObj].Render(m_pImmediateContext);
    }*/
   
    return false;
}

bool Sample::Release()
{
    m_Map.Release();
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Release();
    }*/
    m_Camera.Release();
    return false;
}

Sample::Sample()
{
}
Sample::~Sample()
{
}

TBASIS_RUN(SampleLibrary)