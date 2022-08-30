#include "Sample.h"
#include "TGameCore.cpp"
TSpacePartition* GetInstance(int iType)
{
    if (iType == T_OCTREE) return new TOctree;
    if (iType == T_QUADTREE) return new TQuadtree;
    return nullptr;
}

bool	Sample::Init()
{ 
    m_object.SetDevice(m_pd3dDevice, m_pImmediateContext);
    m_object.Init();

#ifdef OCTREE
    TGameCore<TObject>game;
    game.m_pWorldSP = GetInstance(T_OCTREE);
    game.m_pPlayer = new TGamePlayer;
    game.m_pPlayer->SetPosition(TVector(50, 50, 50), TVector(50, 50, 50));
    game.m_pWorldSP->Create(TVector(0, 0, 0), TVector(100, 100, 0));
    //game.Run();
#else
    TGameCore<TObject2D>game;
    game.m_pWorldSP = GetInstance(T_QUADTREE);
    game.m_pPlayer = new TGamePlayer2D;
    game.m_pPlayer->SetPosition(TVector2D(50, 50), TVector2D(50, 50));
    game.m_pWorldSP->Create(TVector2D(0, 0), TVector2D(100, 100));
    //game.Run();
#endif
    return true;
}
bool		Sample::Frame()
{
    m_object.Frame();
    return true;
}
bool		Sample::Render()
{
    m_object.Render();
    return true;
}
bool		Sample::Release()
{
    m_object.Render();
    return true;
}


GAME_RUN(P2_DrawOctree, 1024, 768)
