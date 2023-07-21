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
    /// 공간을 분할 작업 
    /// </summary>
public:
    void            BuildOctTree(TSpaceData data);
    void            BuildTree(TNode* pParent);
    TNode*          CreateNode(TNode* pParent, TSpaceData data);
    TNode*          CreateNode( TNode* pParent,
                                float x, float y, float z,
                                float w, float h, float d);
    /// <summary>
    /// 객체를 공간에 추가하는 작업
    /// </summary>
public:
    TNode* StaticAddObject(TObject3D* obj);
    TNode* DynamicAddObject(TObject3D* obj);
    TNode* FindNode(TNode* pNode, TObject3D* obj);
    /// <summary>
    /// 분할 된 트리를 순회하는 작업
    /// </summary>
public:
    std::queue<TNode*> g_Queue;
    void            PreOrder(TNode* pNode);
    void            LevelOrder(TNode* pNode);
    /// <summary>
    /// 쿼드트리의 프로세스
    /// </summary>
public:
    bool	        Init();		// 초기화 작업
    bool	        PreFrame();	// 실시간 계산
    bool	        Frame();	// 실시간 계산
    bool	        PostFrame();	// 실시간 계산
    bool	        Render();	// 실시간 랜더링, 드로우
    bool	        Release();	// 객체의 소멸 작업
public:
    TOctree();
    virtual ~TOctree();
};

