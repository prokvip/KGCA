#include "TItem.h"

int    TItem::m_iCounter = 0;
void TItem::Print()
{
   /* printf("%d %d %d %d\n",
        m_iID,
        m_iKor,
        m_iEng,
        m_iMat);*/
}

void TItem::Save(FILE* fp)
{
    fprintf(fp, "%d %d %d %d\n",
        m_iID,
        m_iKor,
        m_iEng,
        m_iMat);
    //fwrite();
}

void TItem::Read(FILE* fp)
{
    fscanf(fp, "%d %d %d %d\n",
        &m_iID,
        &m_iKor,
        &m_iEng,
        &m_iMat);
    //fread();
}

void TItem::Release()
{
}