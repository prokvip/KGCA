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
    bool    Init();     // �ʱ�ȭ	
    bool    Frame(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render();   // ȭ�鿡 ��ο�
    bool    Release();  // �Ҹ�����
    bool	Run();      // ��������
};

