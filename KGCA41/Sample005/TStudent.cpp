#include "TStudent.h"

int    TStudent::m_iCounter = 0;
void    TStudent::Reset()
{
    m_pData = new char[10];
    memset(m_szName, 0, sizeof(char) * 4);
    m_iKor = 0;
    m_iEng = 0;
    m_iMat = 0;
    Compute();
}
void  TStudent::Set(int id)
{
    m_iID = id;
    memset(m_szName, 0, sizeof(char) * 4);
    m_szName[0] = 65 + rand() % 26;
    m_szName[1] = 65 + rand() % 26;
    m_szName[2] = 65 + rand() % 26;
    m_szName[3] = 0;
    m_iKor = rand() % 100;
    m_iEng = rand() % 100;
    m_iMat = rand() % 100;
    Compute();   
}

std::ostream& operator << (std::ostream& os, const TStudent& s)
{
    //os << s.m_iID << " " << s.m_szName;
    s.Print();
    return os;
}
TStudent TStudent::operator + (TStudent& data)
{
    TStudent ret;
    ret.m_iTotal = this->m_iTotal + data.m_iTotal;
    return ret;
}
bool  TStudent::operator == ( TStudent& data) 
{
    /*if (_stricmp(this->m_szName,data.m_szName)!=0)
    {
        return false;
    }*/
    
    for (int i = 0; i < 5; i++)
    {
        //int iS = operator[](i);
        int iS = (*this)[i];        
        int iT = data[i];
        if ( iS != iT)
        {
            return false;
        }
    }
    return true;
}
bool  TStudent::operator != (const TStudent& data) 
{
    if (this->m_iTotal == data.m_iTotal ||
        this->m_iID == data.m_iID ||
        _stricmp(this->m_szName, data.m_szName) == 0)
    {
        return false;
    }
    return true;
}
int   TStudent::operator [](int index)
{
    switch (index)
    {
    case 0: { return m_iID; } break;
    case 1: { return m_iKor; } break;
    case 2: { return m_iEng; } break;
    case 3: { return m_iMat; } break;
    case 4: { return m_iTotal; } break;
    }
    return -1;
}
void   TStudent::Compute()
{
    m_iTotal = m_iKor + m_iEng + m_iMat;
    m_fAverage = m_iTotal / 3.0f;
}
void TStudent::Print() const
{
    printf("%-5d %s %-5d %-5d %-5d %-5d %10.2f\n",
        m_iID,
        m_szName,
        m_iKor,
        m_iEng,
        m_iMat,
        this->m_iTotal,
        this->m_fAverage);
}

void TStudent::Save(FILE* fp)
{
    fprintf(fp, "%d %s %d %d %d\n",
        m_iID,
        m_szName,
        m_iKor,
        m_iEng,
        m_iMat);
    //fwrite();
}

void TStudent::Read(FILE* fp)
{
    fscanf(fp, "%d %s %d %d %d\n",
        &m_iID,
        this->m_szName,
        &m_iKor,
        &m_iEng,
        &m_iMat);
    //fread();
}

void TStudent::Release()
{
}

void  TStudent::operator = (TStudent& data)
{
    std::cout << "대입연산자 호출" << std::endl;
}
TStudent::TStudent(const TStudent& data)
{
    m_fAverage = data.m_fAverage;
    m_iID = data.m_iID;
    memcpy(m_szName, data.m_szName, _countof(data.m_szName));
    
    if (data.m_pData != nullptr)
    {
        int iSize = _msize(data.m_pData);
        m_pData = new char[iSize];
        memcpy(m_pData, data.m_pData, iSize);
    }
    m_iKor = data.m_iKor;
    m_iEng = data.m_iEng;
    m_iMat = data.m_iMat;
    m_iTotal = data.m_iTotal;
    m_fAverage = data.m_fAverage;
    std::cout << "복사 생성자 호출" << std::endl;
}
TStudent::TStudent()
{
    Reset();
    std::cout << "생성자 호출" << std::endl;
}
TStudent::TStudent(int id, const char* szName)
{
    Reset();
    this->m_iID = id;
    this->m_pData = new char[10];
    memset(m_pData, 0, sizeof(char) * 10);
    if (szName != nullptr)
    {
        memcpy(m_pData, szName, sizeof(char) * 10);
        memcpy(m_szName, szName, strlen(szName));
    }
    std::cout << "생성자 호출" << std::endl;
};
void  TStudent::operator = (TStudent&& data)
{
    m_iID = data.m_iID;
    m_pData = data.m_pData;
    data.m_pData = nullptr;
    std::cout << "이동 대입 연산자 호출" << std::endl;
}
TStudent::TStudent(TStudent&& data )
{
    m_iID = data.m_iID;
    m_pData = data.m_pData;
    data.m_pData = nullptr;
    std::cout << "이동 생성자 호출" << std::endl;
}
TStudent::~TStudent()
{
    std::cout << "해제자 호출" << std::endl;
}