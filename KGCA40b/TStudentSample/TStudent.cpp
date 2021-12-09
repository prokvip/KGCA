#include "TStudent.h"

TUser* TStudent::NewNode()
{
	TUser* pUser = new TUser(77,88);
	pUser->iIndex = g_iMaxUserCounter;
	pUser->iKor = rand() % 100;
	pUser->pNext = NULL;
	return pUser;
}
void TStudent::AddLink(TUser* pUser)
{
	if (g_pHeadUserList == NULL)
	{
		g_pHeadUserList = pUser;
		g_pEndUser = pUser;
		g_iMaxUserCounter++;
		return;
	}
	g_pEndUser->pNext = pUser;
	g_pEndUser = pUser;
	g_iMaxUserCounter++;
}
bool TStudent::FileSave()
{
	FILE* fpWrite  = fopen("test2.txt", "wb");
	// 블럭단위(덩어리) 입출력 함수
	int iCouner = g_iMaxUserCounter;
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (TUser* user = g_pHeadUserList;
		user != NULL;
		user = user->pNext)
	{
		fwrite(user, sizeof(TUser), 1, fpWrite);
	}		
	fclose(fpWrite);		
	return true;
}
void TStudent::Create()
{
	for (int iData = 0; iData < 1; iData++)
	{
		AddLink(NewNode());
	}
}
void TStudent::DeleteAll()
{
	TUser* pNext = g_pHeadUserList;
	while (pNext)
	{
		TUser* pDeleteUser = pNext;
		pNext = pDeleteUser->pNext;
		delete pDeleteUser;
		pDeleteUser = NULL;
		g_iMaxUserCounter--;
	}
	g_pHeadUserList = NULL;
}
int TStudent::FileSaveInsert(FILE* fp)
{
	TUser* pSaveEndNode = g_pEndUser;
	for (int iData = 0; iData < 2; iData++)
	{
		AddLink(NewNode());
	}

	fwrite(&g_iMaxUserCounter, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (TUser* user = pSaveEndNode->pNext;
		user != NULL;
		user = user->pNext)
	{
		fwrite(user, sizeof(TUser), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return g_iMaxUserCounter;
}
void TStudent::Load()
{
	FILE* fpRead = fopen("test2.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int iAdd = 0; iAdd < iCounerRead; iAdd++)
		{
			TUser* pUser = new TUser(0,0);
			memset(pUser, 0, sizeof(TUser));
			fread(pUser, sizeof(TUser), 1, fpRead);
			pUser->pNext = 0;
			AddLink(pUser);
		}
		fclose(fpRead);
		fpRead = NULL;
	}
}
void TStudent::Draw()
{
	for (TUser* user = g_pHeadUserList;
		user != NULL;
		user = user->pNext)
	{
		printf("%d %d\n", user->iIndex, user->iKor);
	}
}