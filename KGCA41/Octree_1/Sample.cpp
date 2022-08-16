#include "TGameCore.h"
int main()
{    
    TGameCore game;      
    game.m_pWorldSP = new TOctree;
    //game.m_Player.SetPosition(50,50,10,10);
    game.m_pWorldSP->Create(TVector(0,0,0), TVector(100,100,100));
    game.Run();
    std::cout << "Game Ending!\n";
}
