#pragma once
#include <iostream>
#include <conio.h>
//class THuman
//{
//public:
//    char    m_szName[4];
//    THuman()
//    {
//        std::cout << "THuman";
//    }
//    virtual ~THuman()
//    {
//        std::cout << "THuman";
//    }
//};
class TStudent //: public THuman
{
public:   
    int     m_iID;
    char    m_szName[4];
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
public:
    TStudent()
    {
        std::cout << "TStudent";
    }
    ~TStudent()
    {
        std::cout << "THuman";
    }
};

