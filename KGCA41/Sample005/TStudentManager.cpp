#include "TStudentManager.h"
int   TStudentManager::m_iTotalCounter=0;
bool TStudentManager::Init()
{
    m_LinkedList.Init();
    return true;
}
void TStudentManager::SaveFile(const char* filename)const
{
    TFileIO fileIO;
    FILE* fpWrite = fileIO.CreateFile(filename);
    if (fpWrite != NULL)
    {
        TNode<TStudent>* pFindNode = NULL;
        for (TNode<TStudent>* pNode = m_LinkedList.m_pHead.m_pNext;
            pNode != NULL;
            pNode = pNode->m_pNext)
        {
            pNode->m_pData->Save(fpWrite);
        }
        fileIO.CloseFile();
    }
};
void TStudentManager::LoadFile(const char* filename)
{
    m_LinkedList.Release();

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
            TNode<TStudent>* pNewNode = m_LinkedList.NewNode();           
            // todo
            pNewNode->m_pData = new TStudent;
            _ASSERT(pNewNode->m_pData);
            pNewNode->m_pData->Read(fpRead);
            pNewNode->m_pData->Compute();
            m_LinkedList.Push_Front(pNewNode);
        }
        fileIO.CloseFile();
    }
}

TStudent* TStudentManager::NewStudent()
{
    // ������ ����
    TStudent* node = new TStudent();
    node->Set(m_iTotalCounter++);
    return node;
}
bool  TStudentManager::Run()
{
    int iSelect;
    while (1)
    {
        printf("\n���(0), ����(1), �ε�(2), �ʱⰪ(3), �˻�(4), ����(9)");
        scanf("%d", &iSelect);

        if (iSelect == 9)
        {
            break;
        }
        switch (iSelect)
        {
        case Find:
        {
            const TNode<TStudent>* find = m_LinkedList.Find(3);
        }break;
        case Save:
        {
            /*char filename[20] = { 0, };
            printf("\n���ϴ� ���ϸ��� �Է��ϼ��� =");
            scanf("%s", filename);*/
            SaveFile("demo.txt");
        }break;
        case Load:
        {
            LoadFile("demo.txt");
        }break;
        case Print:
        {
           // m_LinkedList.ForwardPrint();
            std::cout << m_LinkedList;
        }break;
        case Create:
        {
            for (int iNode = 0; iNode < 10; iNode++)
            {
                TNode<TStudent>* pNewNode = m_LinkedList.NewNode();
                pNewNode->m_pData = NewStudent();
                m_LinkedList.Push_Front(pNewNode);
            }
            /*std::cout << m_LinkedList;
            TNode<TStudent>* ret = m_LinkedList[5];
            std::cout << *(ret->m_pData);*/
        }break;
        }
    }

    Release();
    return true;
}

void TStudentManager::Release()
{
    m_LinkedList.Release();
}