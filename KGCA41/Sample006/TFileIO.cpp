#include "TFileIO.h"
FILE* TFileIO::CreateFile(const char* filename, 
                          const char* fpMode)
{
    m_pFP = fopen(filename, fpMode);
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

void TFileIO::Release()
{
}
