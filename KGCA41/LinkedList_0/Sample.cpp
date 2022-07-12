#include "Sample.h"
void Clear()
{
	system("cls");
	printf("===================================\n");
	printf("====   ���� ���� ���α׷�      ====\n");
	printf("===================================\n");
	printf("�̸�\t��ȣ\t����\t����\t����\t����\t���\n");
}
void main()
{	
	srand(time(NULL));
	Init();
	int iWork = 1;
	while (iWork)
	{
		int iSelect = 0;
		printf("\n->[0:Create],[1:Print],[2:Find],[3:Del],[4:insert],[Quit:999]  : ");
		scanf("%d", &iSelect);
		if (iSelect == 999) break;

		Clear();

		switch (iSelect)
		{
			case 0:
			{				
				InitData(3);			
			}break;
			case 1:
			{
				AllPrint();
			}break;
			case 2:
			{
				TStudent* pFind = Find(3);
				if (pFind != nullptr)
				{
					Print(pFind);
				}
			}break;
			case 3 :
			{
				TStudent* pFind = Find(2);
				if (pFind != nullptr)
				{
					Print(pFind);
					erase(pFind);
					system("cls");
					AllPrint();
				}
			}break;
			case 4:
			{
				push_back(NewNode());
			}break;
			case 5:
			{
				TStudent* pFind = Find(2);
				if (pFind != nullptr)
				{
					push_front(NewNode(), pFind);
					AllPrint();
				}
			}break;
		}
	}
	
	clear();
	Release();	
}