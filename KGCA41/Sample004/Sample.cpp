#include "TLinkedlist.h"

//printf("\n출력(0), 저장(1), 로드(2), 초기값(3), 종료(9)");
enum work { Print=0, Save, Load, Create, Find, Exit};
int main()
{   
    TLinkedlist linkedlist;
    linkedlist.Init();
    
    int iSelect;
    while(1)
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
                const TBox* find = linkedlist.Find(3);
            }break;
            case Save: {
                /*char filename[20] = { 0, };
                printf("\n원하는 파일명을 입력하세여 =");
                scanf("%s", filename);*/
                linkedlist.Save("demo.txt");
            }break;
            case Load: {
                linkedlist.Load("demo.txt");
            }break;
            case Print:
            {
                linkedlist.ForwardPrint();
            }break;
            case Create:
            {
                for (int iNode = 0; iNode < 10; iNode++)
                {
                    TBox* pNewNode = linkedlist.NewNode();
                    linkedlist.Push_Front(pNewNode);
                }
            }break;        
        }
    }

    printf("아무키나 누르시오!");
    _getche();
}

