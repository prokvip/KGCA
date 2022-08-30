#include "Sample.h"
//#include "TGameCore.cpp"
TSpacePartition* GetInstance(int iType)
{
    if (iType == T_OCTREE) return new TOctree;
    if (iType == T_QUADTREE) return new TQuadtree;
    return nullptr;
}

bool	Sample::Init()
{ 
    m_object.SetDevice(m_pd3dDevice, m_pImmediateContext);
    m_object.Create();

#ifdef OCTREE
    game3D.m_pWorldSP = GetInstance(T_OCTREE);
    game3D.m_pPlayer = new TGamePlayer;
    game3D.m_pPlayer->SetPosition(TVector(50, 50, 50), TVector(50, 50, 50));
    game3D.m_pWorldSP->Create(TVector(0, 0, 0), TVector(100, 100, 0));
#else
    game2D.m_pd3dDevice = m_pd3dDevice;
    game2D.m_pImmediateContext = m_pImmediateContext;
    game2D.m_pWorldSP = GetInstance(T_QUADTREE);
    game2D.m_pPlayer = new TGamePlayer2D;
    game2D.m_pPlayer->GetImp()->SetDevice(m_pd3dDevice, m_pImmediateContext);
    game2D.m_pPlayer->GetImp()->Create();
    game2D.m_pPlayer->GetImp()->Init();
    game2D.m_pPlayer->SetPosition(TVector2D(75, 95), TVector2D(50, 50));
    game2D.m_pWorldSP->Create(TVector2D(0, 0), TVector2D(100, 100));
    game2D.Init();
#endif
    return true;
}
bool		Sample::Frame()
{
    //m_object.Frame();
    game2D.Frame(m_fElapseTimer, m_fGameTimer);
    return true;
}
bool		Sample::Render()
{
    //m_object.Render();
    game2D.Render();
    return true;
}
bool		Sample::Release()
{
    //m_object.Release();
    return true;
}


GAME_RUN(P2_DrawOctree, 1024, 768)
