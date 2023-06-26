#include "TItemManager.h"
int   TItemManager::m_iTotalCounter = 0;
bool TItemManager::Init()
{
    m_LinkedList.Init();
    return true;
}
void TItemManager::SaveFile(const char* filename)const
{
    TFileIO fileIO;
    FILE* fpWrite = fileIO.CreateFile(filename);
    if (fpWrite != NULL)
    {
        TNode<TItem>* pFindNode = NULL;
        for (TNode<TItem>* pNode = m_LinkedList.m_pHead.m_pNext;
            pNode != NULL;
            pNode = pNode->m_pNext)
        {
            pNode->m_pData->Save(fpWrite);
        }
        fileIO.CloseFile();
    }
};
void TItemManager::LoadFile(const char* filename)
{
    TFileIO fileIO;
    FILE* fpRead = fileIO.CreateFile(filename, "r");
    if (fpRead != NULL)
    {
        while (1)
        {
            if (feof(fpRead))
            {
                break;
            }
            TNode<TItem>* pNewNode = m_LinkedList.NewNode();
            // todo
            pNewNode->m_pData = new TItem;
            pNewNode->m_pData->Read(fpRead);
            m_LinkedList.Push_Front(pNewNode);
        }
        fileIO.CloseFile();
    }
}

TItem* TItemManager::NewStudent()
{
    // 데이터 생성
    TItem* node = new TItem;
    node->m_iID = m_iTotalCounter++;
    node->m_iKor = rand() % 100;
    node->m_iEng = rand() % 100;
    node->m_iMat = rand() % 100;
    node->m_pNext = NULL;
    return node;
}
bool  TItemManager::Run()
{
    int iSelect;
    while (1)
    {
        printf("\n출력(0), 저장(1), 로드(2), 초기값(3), 검색(4), 종료(9)");
        scanf("%d", &iSelect);

        if (iSelect == 9)
        {
            break;
        }
        switch (iSelect)
        {
        case Find:
        {
            const TNode<TItem>* find = m_LinkedList.Find(3);
        }break;
        case Save:
        {
            /*char filename[20] = { 0, };
            printf("\n원하는 파일명을 입력하세여 =");
            scanf("%s", filename);*/
            SaveFile("demo.txt");
        }break;
        case Load:
        {
            LoadFile("demo.txt");
        }break;
        case Print:
        {
            m_LinkedList.ForwardPrint();
        }break;
        case Create:
        {
            for (int iNode = 0; iNode < 10; iNode++)
            {
                TNode<TItem>* pNewNode = m_LinkedList.NewNode();
                pNewNode->m_pData = NewStudent();
                m_LinkedList.Push_Front(pNewNode);
            }
        }break;
        }
    }

    Release();
    return true;
}

void TItemManager::Release()
{
    m_LinkedList.Release();
}