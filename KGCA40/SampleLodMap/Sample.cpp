#include "Sample.h"
// 1)리프노드 랜더링
//   - 공유 정점버퍼(맵), 리프노드 당 인덱스 버퍼 사용
// 2)리프노드 랜더링
//   - 리프노드 당 정점버퍼, 공유되는 인덱스 버퍼 사용
//   = LOD -> 패치단위로 16개의 정점 인덱스 버퍼가 있어야 된다. 
//  LOD
// 1) 리프노드에 패치 몇개야 ? 
//   - PatchList[][16] 인덱스버퍼가 있다.
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
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
    m_Quadtree.Build(&m_Map);
    
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
    //m_Map.Render(m_pImmediateContext);
    m_Quadtree.Render(m_pImmediateContext);
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