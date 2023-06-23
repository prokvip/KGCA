#pragma once
//#define _CRT_SECURE_NO_WARNINGS
/*
*  후위 연결리스트 && 양방향 구현
*  전위 연결리스트 && 양방향 구현
*/
#include <iostream>
#include <conio.h>
#include "TStudent.h"
using namespace std;

struct TBox
{
    TStudent* m_pData;
    TBox* m_pNext;
    TBox* m_pPrev;
    virtual ~TBox()
    {
        delete m_pData;
        m_pData = nullptr;
    }
};

class TLinkedlist
{
public:
    TBox  m_pHead;
    TBox* m_pEndNode = NULL;
    static int    m_iCounter;
public:
     static void  Set(const TLinkedlist* list);
     TBox*  NewNode();
    void    Push_Back(TBox* pNewNode);
    void    Push_Front(TBox* pNewNode);
    const   TBox*  Find(int id);
    void    ForwardPrint() const;
    void    BackwardPrint()const;
    void    ForwardPrint(TBox* pNode)const;
    void    BackwardPrint(TBox* pNode)const;

public:
    bool    Init();
    bool    Release();
};
