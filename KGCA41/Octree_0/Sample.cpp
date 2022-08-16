#include "TGameCore.h"
int main()
{    
    TGameCore game;      
    game.m_pWorldSP = new TQuadtree;
    game.m_Player.SetPosition(50,50,10,10);
    game.m_pWorldSP->Create(100,100);
    game.Run();
    std::cout << "Game Ending!\n";
}
