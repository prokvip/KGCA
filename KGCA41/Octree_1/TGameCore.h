#pragma once
#include "TQuadtree.h"
#include "TOctree.h"
#include "TGamePlayer.h"
#include "TEnemy.h"
#define   OCTREE
class TGameCore
{
public:
    TGamePlayer             m_Player;   
    TGamePlayer2D           m_Player2D;
public:
    TSpacePartition*        m_pWorldSP;
    std::map<int, TBaseObject*> m_npcList;
    std::map<int, TBaseObject*> m_AllObjectList;
    std::vector<TBaseObject*>   m_DrawList;
public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

