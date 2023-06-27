#include "TStudent.h"

int    TStudent::m_iCounter = 0;
void    TStudent::Reset()
{
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

TStudent::TStudent()
{
    Reset();
}
TStudent::~TStudent()
{
    std::cout << "THuman";
}