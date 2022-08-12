#pragma once
#include "TQuadtree.h"
#include "TGamePlayer.h"
#include "TMapObject.h"
#include "TNonCharacter.h"
class TGameCore
{
public:
    TGamePlayer             player;   
    TQuadtree               quadtree;
    std::map<int, TObject*> npcList;
    std::map<int, TObject*> AllObjectList;
    std::vector<TObject*>   DrawList;
public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

