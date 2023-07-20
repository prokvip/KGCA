#pragma once
#include "TNode.h"
class TOctree
{
public:
    TNode* m_pRootNode = nullptr;
    int                 m_iNumNodeCounter = 0;
    std::set<TNode*>    m_DynamicObjectNodeList;
    std::vector<TNode*> m_NodeArrayList;
    /// <summary>
    /// ������ ���� �۾� 
    /// </summary>
public:
    void            BuildOctTree(float x, float y, float w, float h);
    void            BuildTree(TNode* pParent);
    TNode* CreateNode(TNode* pParent, float x,
        float y,
        float fWidth,
        float fHeight);
    /// <summary>
    /// ��ü�� ������ �߰��ϴ� �۾�
    /// </summary>
public:
    TNode* StaticAddObject(TObject* obj);
    TNode* DynamicAddObject(TObject* obj);
    TNode* FindNode(TNode* pNode, TObject* obj);
    /// <summary>
    /// ���� �� Ʈ���� ��ȸ�ϴ� �۾�
    /// </summary>
public:
    std::queue<TNode*> g_Queue;
    void            PreOrder(TNode* pNode);
    void            LevelOrder(TNode* pNode);
    /// <summary>
    /// ����Ʈ���� ���μ���
    /// </summary>
public:
    bool	        Init();		// �ʱ�ȭ �۾�
    bool	        PreFrame();	// �ǽð� ���
    bool	        Frame();	// �ǽð� ���
    bool	        PostFrame();	// �ǽð� ���
    bool	        Render();	// �ǽð� ������, ��ο�
    bool	        Release();	// ��ü�� �Ҹ� �۾�
public:
    TOctree();
    virtual ~TOctree();
};

