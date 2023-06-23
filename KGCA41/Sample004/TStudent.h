#pragma once
#include <iostream>
#include <conio.h>
class TStudent
{
public:
    char    m_szName[4];
    int     m_iID;
    int     m_iKor;
    int     m_iEng;
    int     m_iMat;
    int     m_iTotal;
    int     m_iAverage;
    TStudent* m_pNext;
    TStudent* m_pPrev;
    static int    m_iCounter;
public:
    void    Print();
    void    Save(FILE* fp);
    void    Read(FILE* fp);
    void    Release();
    TStudent* NewNode();
};

