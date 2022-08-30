#pragma once
#include "TQuadtree.h"
#include "TOctree.h"
#include "TGamePlayer.h"
#include "TEnemy.h"
//#define   OCTREE
template<class T>
class TGameCore
{
public:
public:
    ID3D11Device*           m_pd3dDevice = nullptr;// 디바이스 객체
    ID3D11DeviceContext*    m_pImmediateContext = nullptr;
    TPlayer<T>*             m_pPlayer;
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

