#include "TGameCore.h"
TSpacePartition* GetInstance(int iType)
{
    if (iType == T_OCTREE) return new TOctree;
    if (iType == T_QUADTREE) return new TQuadtree;
    return nullptr;
}
int main()
{    
    TGameCore game;      
#ifdef OCTREE
    game.m_pWorldSP = GetInstance(T_OCTREE);
    game.m_Player.SetPosition(TVector(50,50,50), TVector(50,50,50));
    game.m_pWorldSP->Create(TVector(0,0,0), TVector(100,100,0));
#else
    game.m_pWorldSP = GetInstance(T_QUADTREE);
    game.m_Player2D.SetPosition(TVector2D(50, 50), TVector2D(50, 50));
    game.m_pWorldSP->Create(TVector2D(0, 0), TVector2D(100, 100));
#endif
    game.Run();
    std::cout << "Game Ending!\n";
}
