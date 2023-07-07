#include <iostream>
#include <string.h>
#include <string>
class TString
{
public:
    TString();
    TString(const char* s);
    TString(const TString& s);
    ~TString();
public:
    int   size();
    const char* GetData() const;
public:
    void operator = (const TString& s);
    TString& operator += ( TString& s);
    TString  operator + ( TString& s);
    bool operator == ( TString& s);
    char operator [] (int index);
public:
    char*   m_pBuffer=nullptr;
    int     m_iSize;
};
TString::TString()
{
}
TString::TString(const char* s )
{
    m_iSize = strlen(s);
    m_pBuffer = new char[m_iSize+1];
    strcpy_s(m_pBuffer, m_iSize+1, s);
}
TString::TString(const TString& s)
{
    m_iSize = s.m_iSize;
    m_pBuffer = new char[m_iSize + 1];
    strcpy_s(m_pBuffer, m_iSize + 1, s.GetData());
}
TString::~TString()
{
    delete m_pBuffer;
    m_pBuffer = nullptr;
}

int   TString::size()
{
    return m_iSize;
}
const char* TString::GetData() const
{
    return m_pBuffer;
}

void TString::operator = (const TString& s)
{
    if (m_pBuffer) delete m_pBuffer;
    m_iSize = s.m_iSize;
    m_pBuffer = new char[m_iSize+1];   
    strcpy_s(m_pBuffer,m_iSize+1, s.GetData());
}
TString& TString::operator += ( TString& s)
{       
    int iAddSize = m_iSize + s.size();
    char* addBuffer = new char[iAddSize+1];
    memset(addBuffer, 0, sizeof(char) * iAddSize + 1);
        int error = strcpy_s(addBuffer, iAddSize+1, m_pBuffer);
        error = strcpy_s(&addBuffer[m_iSize], iAddSize+1- m_iSize, s.GetData());
        if (m_pBuffer) delete m_pBuffer;    
        m_pBuffer = new char[iAddSize + 1];
        memset(m_pBuffer, 0, sizeof(char) * iAddSize + 1);
        strcpy_s(m_pBuffer, iAddSize + 1, addBuffer);
    delete addBuffer;
    m_iSize = iAddSize;
    return *this;
}
TString TString::operator + ( TString& s)
{
    TString add(m_pBuffer);
    add += s;
    return add;
}
bool TString::operator == ( TString& s)
{
    if (strcmp(m_pBuffer, s.GetData())==0)
    {
        return true;
    }
    return false;
}
char TString::operator [] (int index)
{
    return m_pBuffer[index];
}
int main()
{
    TString s1("kgca");
    TString s2("game");
    TString s3("kgca");
    if (s1 == s3)
    {
        std::cout << "s1 == s2" << "\n";
    }
    s1 += s2;
    TString s5;
    s5 = s1 + s2;
    TString s6 = s5 + s2 + s3;

    std::cout << s6.GetData() << "\n";

    std::string t1;
    std::wstring t2;

    std::basic_string<char> t3;
    std::basic_string<wchar_t> t4;
}
