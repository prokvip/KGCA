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
class TItem 
{
public:
    int     m_iID;
    char    m_szName[4];            
    static int    m_iCounter;
public:
    void    Print();
    void    Save(FILE* fp);
    void    Read(FILE* fp);
    void    Release();
    TItem* NewNode();
public:
    TItem()
    {
        std::cout << "TStudent";
    }
    ~TItem()
    {
        std::cout << "THuman";
    }
};

