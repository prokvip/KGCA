#pragma once
#include "TQuadtree.h"
#include "TOctree.h"
#include "TGamePlayer.h"
//#include "TMapObject2d.h"
#include "TEnemy.h"
class TGameCore
{
public:
    TGamePlayer             m_Player;   
    TSpacePartition*        m_pWorldSP;
    std::map<int, TObject*> m_npcList;
    std::map<int, TObject*> m_AllObjectList;
    std::vector<TObject*>   m_DrawList;

    std::map<int, TObject2D*> m_npcList2D;
    std::map<int, TObject2D*> m_AllObjectList2D;
    std::vector<TObject2D*>   m_DrawList2D;
public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

