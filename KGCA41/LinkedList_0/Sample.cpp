#include "TStudentManager.h"
#include <iostream>

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
	TStudentManager mgr;
	
	srand(time(NULL));
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
				mgr.InitData(3);
			}break;
			case 1:
			{
				std::cout << mgr;//	mgr.AllPrint();
			}break;
			case 2:
			{
				TStudent* pFind = mgr.m_List.Find(3);
				if (pFind != nullptr)
				{
					mgr.Print(pFind);
				}
			}break;
			case 3 :
			{
				TStudent* pFind = mgr.m_List.Find(3);
				if (pFind != nullptr)
				{
					mgr.Print(pFind);
					mgr.m_List.erase(pFind);
					system("cls");
					mgr.AllPrint();
				}
			}break;
			case 4:
			{
				mgr.m_List.push_back(mgr.NewNode());
			}break;
			case 5:
			{
				TStudent* pFind = mgr.m_List.Find(2);
				if (pFind != nullptr)
				{
					mgr.m_List.push_front(mgr.NewNode(), pFind);
					mgr.AllPrint();
				}
			}break;
		}
	}	
	mgr.m_List.clear();
}