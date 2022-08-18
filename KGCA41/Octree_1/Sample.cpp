#include "TGameCore.h"
int main()
{    
    TGameCore game;      
#ifdef OCTREE
    game.m_pWorldSP = new TOctree;
    game.m_Player.SetPosition(TVector(50,50,50), TVector(50,50,50));
    game.m_pWorldSP->Create(TVector(0,0,0), TVector(100,100,0));
#else
    game.m_pWorldSP = new TQuadtree;
    game.m_Player2D.SetPosition(TVector2D(50, 50), TVector2D(50, 50));
    game.m_pWorldSP->Create(TVector2D(0, 0), TVector2D(100, 100));
#endif
    game.Run();
    std::cout << "Game Ending!\n";
}
