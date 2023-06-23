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

TStudent* TStudent::NewNode()
{
    // 데이터 생성
    TStudent* node = new TStudent;
    node->m_iID = m_iCounter++;
    node->m_iKor = rand() % 100;
    node->m_iEng = rand() % 100;
    node->m_iMat = rand() % 100;
    node->m_pNext = NULL;
    return node;
}