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
    bool    Init();     // �ʱ�ȭ	
    bool    Frame(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render();   // ȭ�鿡 ��ο�
    bool    Release();  // �Ҹ�����
    bool	Run();      // ��������
};

