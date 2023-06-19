#include "Linkedlist.h"

//error C4996 : 'fopen' : This function or variable may be unsafe.
// Consider using fopen_s instead.To disable deprecation, 
// use _CRT_SECURE_NO_WARNINGS.See online help for details.
int main()
{   
    g_pEndNode = &g_pHead;
    g_pHead.iID = -1; g_pHead.iValue = -1;
    g_pHead.pNext = NULL;
    g_pHead.pPrev = NULL;

   
    
    int iSelect;
    while(1)
    {
        printf("\n출력(2), 저장(0), 로드(1), 초기값(3), 종료(9)");
        scanf("%d", &iSelect);

        if (iSelect == 9)
        {
            break;
        }
        switch (iSelect)
        {
        case 0: {
            FILE* fpWrite = fopen("demo.txt", "w");
            if (fpWrite != NULL)
            {
                TNode* pFindNode = NULL;
                for (TNode* pNode = g_pHead.pNext;
                    pNode != NULL;
                    pNode = pNode->pNext)
                {
                    fprintf(fpWrite, "%d %d\n", pNode->iID, pNode->iValue);
                    //fwrite();
                }                
                fclose(fpWrite);
            }
        }
              break;
        case 1: {
            FILE* fpRead = fopen("demo.txt", "r");
            if (fpRead != NULL)
            {
                //for (int i = 0; i < 10; i++)
                while(1)
                {                    
                    if (feof(fpRead))
                    {
                        break;
                    }
                    TNode* pNewNode = NewNode();
                    fscanf(fpRead, "%d %d\n", &pNewNode->iID, &pNewNode->iValue);
                    //fread();
                    Push_Front(pNewNode);
                }
                fclose(fpRead);
            }
        }
              break;
        case 2: { 
            ForwardPrint(); 
        case 3: {
            for (int iNode = 0; iNode < 10; iNode++)
            {
                TNode* pNewNode = NewNode();
                Push_Front(pNewNode);
            }
        }break;
        }break;
        }
    }


    //for (int iNode = 0; iNode < 10; iNode++)
    //{
    //    TNode* pNewNode = NewNode();
    //    Push_Front(pNewNode);
    //}
    //for (int iNode = 0; iNode < 10; iNode++)
    //{
    //    TNode* pNewNode = NewNode();
    //    Push_Back(pNewNode);
    //}
    //BackwardPrint();
    //ForwardPrint();
    //// 검색 출력
    //BackwardPrint(Find(10));
    //ForwardPrint(Find(1879670));

    //TNode* pNode = g_pHead.pNext;
    //while (pNode != NULL)
    //{
    //    TNode* pNext = pNode->pNext;
    //    free(pNode);
    //    pNode = pNext;
    //}

    // 탐색, 삭제, 수정, 정렬, 저장, 불러오기, 삽입, 소멸
    /*FILE* fpWrite = fopen("demo.txt", "w");
    if (fpWrite != NULL)
    {
        fprintf(fpWrite, "%d %d\n", 100, 200);
        fprintf(fpWrite, "%d %d\n", 300, 400);
        fclose(fpWrite);
    }

    FILE* fpRead = fopen("demo.txt", "r");
    if (fpWrite != NULL)
    {
        int a = 0;
        int b = 0;
        fscanf(fpRead, "%d %d\n", &a, &b);
        fscanf(fpRead, "%d %d\n", &a, &b);
        fclose(fpRead);
    }*/

    printf("아무키나 누르시오!");
    _getche();
}

