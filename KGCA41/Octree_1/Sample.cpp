#include "TGameCore.h"
int main()
{    
    TGameCore game;      
    game.m_pWorldSP = new TOctree;
    //game.m_pWorldSP = new TQuadtree;
    game.m_Player.SetPosition(TVector(50,50,50), TVector(50,50,50));
    game.m_pWorldSP->Create(TVector(0,0,0), TVector(100,100,0));

    game.Run();
    std::cout << "Game Ending!\n";
}
