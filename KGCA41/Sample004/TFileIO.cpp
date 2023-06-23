#include "TFileIO.h"
FILE* TFileIO::CreateFile(const char* filename)
{
    m_pFP = fopen(filename, "w");
    if (m_pFP != nullptr)
    {       
        return m_pFP;
    }
    return m_pFP;
};
void TFileIO::CloseFile()
{
    fclose(m_pFP);
}