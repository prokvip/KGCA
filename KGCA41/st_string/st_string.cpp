#include <iostream>
#include <string.h>
#include <string>
#include <atlconv.h> // A2W

static std::wstring to_mw(const std::string& _src)
{
    USES_CONVERSION;
    return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
    USES_CONVERSION;
    return std::string(W2A(_src.c_str()));
};

class TString
{
public:
    TString();
    TString(const char* s);
    TString(const TString& s);
    TString( TString&& s);
    ~TString();
public:
    int   size();
    const char* GetData() const;
public:
    void operator = (const TString& s);
    void operator = ( TString&& s);
    TString& operator += ( TString& s);
    TString  operator + ( TString& s);
    bool operator == ( TString& s);
    char operator [] (int index);
    friend std::ostream& operator << (std::ostream& os, const TString& s);
    friend std::istream& operator >> (std::istream& is, TString& s);
private:
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
TString::TString( TString&& s)
{
    m_iSize = s.m_iSize;
    m_pBuffer = s.m_pBuffer;
    s.m_pBuffer = nullptr;
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
void TString::operator = (TString&& s)
{
    if (this == &s)
    {
        return;
    }
    if (m_pBuffer) delete m_pBuffer;
    m_iSize = s.m_iSize;
    m_pBuffer = s.m_pBuffer;
    s.m_pBuffer = nullptr;
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

std::ostream& operator << (std::ostream& os, const TString& s)
{
    os << s.GetData();
    return os;
}
std::istream& operator >> (std::istream& is, TString& s)
{
    is >> s.m_pBuffer;
    return is;
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
    std::cout << s6 << "\n";

    std::basic_string<char> s0 = "game";
    std::basic_string<wchar_t> u1 = L"kgca";
    std::basic_string<wchar_t> u2 = L"한글";    
    s0 = "kgca" + s0;
    std::wstring u3 = L"한글" + u1;
     //if(u1 == u2) 
    //if( u1 < u2)
    const char* name = "kgca";
    std::string st0 = name;
    std::cout << st0 << "\n";
    const wchar_t* name1 = L"kgca";
    std::wstring st1 = name1;
    std::wcout << st1 << "\n";

    //std::string tmp = "한글";
    //std::string t1 = "kgca" + tmp;
    //int itmpsize1 = t1.size();
    //std::cout << t1.c_str() << "\n";
    //// k,g,c,d,하,안,그,글
    //std::wstring t2;
    //std::wstring tmp2 = L"한글";
    //std::wstring t3 = L"kgca" + tmp2;
    //int itmpsize2 = t3.size();
    //// k+0, g+0, c+0, d+0, 한, 글   
    //wchar_t unicode[100] = { 0, };
    //memcpy(unicode, t3.c_str(), t3.size()*2);

    //std::string multibyte = to_wm(t3);
    //std::wstring unicodebyte = to_mw(multibyte);
    //int a = multibyte.size();
    //int b = unicodebyte.size();
    //int iA = strlen(multibyte.c_str());
    //int iB = strlen(to_wm(unicodebyte).c_str());

    //const char* cData = multibyte.c_str();
    //printf("%s",cData);
    //std::basic_string<char> t43;
    //std::basic_string<wchar_t> t44;    
}
