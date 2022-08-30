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
    ID3D11Device*           m_pd3dDevice = nullptr;// ����̽� ��ü
    ID3D11DeviceContext*    m_pImmediateContext = nullptr;
    TPlayer<T>*             m_pPlayer;
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

