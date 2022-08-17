#pragma once
//#include "TQuadtree.h"
#include "TOctree.h"
//#include "TGamePlayer2D.h"
//#include "TMapObject2d.h"
#include "TEnemy.h"
class TGameCore
{
public:
    //TGamePlayer2D           m_Player2D;   
    TSpacePartition*        m_pWorldSP;
    std::map<int, TObject*> m_npcList;
    std::map<int, TObject*> m_AllObjectList;
    std::vector<TObject*>   m_DrawList;
public:
    bool    Init();     // �ʱ�ȭ	
    bool    Frame(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render();   // ȭ�鿡 ��ο�
    bool    Release();  // �Ҹ�����
    bool	Run();      // ��������
};

