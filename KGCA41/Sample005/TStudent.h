#pragma once
#include "TFileIO.h"
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
    int         m_iID;     
    char        m_szName[4];
    char*       m_pData = nullptr;    
    int         m_iKor;
    int         m_iEng;
    int         m_iMat;
    int         m_iTotal;
    float       m_fAverage;  
    static int  m_iCounter;
    void  operator = (TStudent& data);
    void  operator = (TStudent&& data);
public:
    bool  operator == ( TStudent& data) ;
    bool  operator != (const TStudent& data) ;
    int   operator [](int index);    
    TStudent operator + (TStudent& data);
    friend std::ostream& operator << (std::ostream& os, const TStudent& s);
public:
    void    Print() const;
    void    Compute();
    void    Save(FILE* fp);
    void    Read(FILE* fp);
    void    Release();
    TStudent* NewNode();
    void    Set(int id);
    void    Reset();
public:
    TStudent();
    TStudent(int id, const char* m_szName=nullptr);
    // ���������
    TStudent(const TStudent& data);
    TStudent(TStudent&& data);
    ~TStudent();
};
