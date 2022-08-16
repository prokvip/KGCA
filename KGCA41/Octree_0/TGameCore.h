#pragma once
#include "TQuadtree.h"
#include "TOctree.h"
#include "TGamePlayer2D.h"
#include "TMapObject2d.h"
#include "TNonCharacter2d.h"
class TGameCore
{
public:
    TGamePlayer2D             m_Player;   
    TSpacePartition*        m_pWorldSP;
    std::map<int, TObject2D*> m_npcList;
    std::map<int, TObject2D*> m_AllObjectList;
    std::vector<TObject2D*>   m_DrawList;
public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

