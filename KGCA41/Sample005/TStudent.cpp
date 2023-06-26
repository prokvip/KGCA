#include "TStudent.h"

int    TStudent::m_iCounter = 0;
void TStudent::Print()
{
    printf("%d %d %d %d\n",
        m_iID,
        m_iKor,
        m_iEng,
        m_iMat);
}

void TStudent::Save(FILE* fp)
{
    fprintf(fp, "%d %d %d %d\n",
        m_iID,
        m_iKor,
        m_iEng,
        m_iMat);
    //fwrite();
}

void TStudent::Read(FILE* fp)
{
    fscanf(fp, "%d %d %d %d\n",
        &m_iID,
        &m_iKor,
        &m_iEng,
        &m_iMat);
    //fread();
}

void TStudent::Release()
{
}