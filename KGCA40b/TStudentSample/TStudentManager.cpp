#include "TStudentManager.h"

 TStudent* const TStudentManager::NewNode()
{
	TStudent* pUser = new TStudent(77, 88);
	pUser->iIndex = g_iMaxUserCounter;
	pUser->iKor = rand() % 100;
	pUser->pNext = NULL;
	return pUser;
}
void TStudentManager::AddLink( TStudent* const pUser)
{
	//pUser = 0; //error
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
bool TStudentManager::FileSave()
{
	FILE* fpWrite = fopen("test2.txt", "wb");
	// 블럭단위(덩어리) 입출력 함수
	int iCouner = g_iMaxUserCounter;
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (TStudent* user = g_pHeadUserList;
		user != NULL;
		user = user->pNext)
	{
		fwrite(user, sizeof(TStudent), 1, fpWrite);
	}
	fclose(fpWrite);
	return true;
}
void TStudentManager::Create()
{
	for (int iData = 0; iData < 1; iData++)
	{
		AddLink(NewNode());
	}
}
void TStudentManager::DeleteAll()
{
	TStudent* pNext = g_pHeadUserList;
	while (pNext)
	{
		TStudent* pDeleteUser = pNext;
		pNext = pDeleteUser->pNext;
		delete pDeleteUser;
		pDeleteUser = NULL;
		g_iMaxUserCounter--;
	}
	g_pHeadUserList = NULL;
}
int TStudentManager::FileSaveInsert(FILE* fp)
{
	TStudent* pSaveEndNode = g_pEndUser;
	for (int iData = 0; iData < 2; iData++)
	{
		AddLink(NewNode());
	}

	fwrite(&g_iMaxUserCounter, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (TStudent* user = pSaveEndNode->pNext;
		user != NULL;
		user = user->pNext)
	{
		fwrite(user, sizeof(TStudent), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return g_iMaxUserCounter;
}
void TStudentManager::Load()
{
	FILE* fpRead = fopen("test2.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int iAdd = 0; iAdd < iCounerRead; iAdd++)
		{
			TStudent* pUser = new TStudent(0, 0);
			memset(pUser, 0, sizeof(TStudent));
			fread(pUser, sizeof(TStudent), 1, fpRead);
			pUser->pNext = 0;
			AddLink(pUser);
		}
		fclose(fpRead);
		fpRead = NULL;
	}
}
void TStudentManager::Draw()
{
	for (TStudent* user = g_pHeadUserList;
		user != NULL;
		user = user->pNext)
	{
		printf("%d %d\n", user->iIndex, user->iKor);
	}
}