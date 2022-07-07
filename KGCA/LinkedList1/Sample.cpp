#include "Sample.h"
void main(int argc, char* argv[])
{
	// 1)������ �ʱ�ȭ �� �Է�
	FILE* fp = Create();
	// 2)�߰�
	//int iCounter = FileSaveInsert(fp);
	fclose(fp);
	DeleteAll();

	////// 3) �ε��ؼ� ����
	Load();
	////// 3) ȭ�����
	Draw(g_iMaxUserCounter);
	DeleteAll();
}
TUser* NewNode()
{
	TUser* pUser = (TUser*)malloc(sizeof(TUser));
	pUser->iIndex = g_iMaxUserCounter;
	pUser->iKor = rand() % 100;
	pUser->pNext = NULL;
	return pUser;
}
void AddLink(TUser* pUser)
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
FILE* Create()
{
	for (int iData = 0; iData < 100; iData++)
	{
		AddLink(NewNode());
	}

	FILE* fpWrite = fopen("test2.txt", "r+b");
	if (fpWrite == NULL)
	{
		fpWrite = fopen("test2.txt", "wb");
		// ������(���) ����� �Լ�
		int iCouner = g_iMaxUserCounter;
		fwrite(&iCouner, sizeof(int), 1, fpWrite);
		for (TUser* user = g_pHeadUserList;
			user != NULL;
			user = user->pNext)
		{
			fwrite(user, sizeof(TUser), 1, fpWrite);
		}		
		fseek(fpWrite, 0, SEEK_SET);
		//fclose(fpWrite);
		//fpWrite = fopen("test2.txt", "r+");
	}
	return fpWrite;
}
void DeleteAll()
{
	TUser* pNext = g_pHeadUserList;
	while(pNext)
	{	
		TUser* pDeleteUser = pNext;
		pNext = pDeleteUser->pNext;
		free(pDeleteUser);
		pDeleteUser = NULL;
	}	
	g_pHeadUserList = NULL;
}
int FileSaveInsert(FILE* fp)
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
void Load()
{
	FILE* fpRead = fopen("test2.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int iAdd=0; iAdd< iCounerRead; iAdd++)
		{
			TUser* pUser = (TUser*)malloc(sizeof(TUser));
			memset(pUser, 0, sizeof(TUser));
			fread(pUser, sizeof(TUser), 1, fpRead);
			pUser->pNext = 0;
			AddLink(pUser);
		}
		fclose(fpRead);
		fpRead = NULL;
	}
}
void Draw(int iCnt)
{
	for (TUser* user = g_pHeadUserList;
		user != NULL;
		user = user->pNext)
	{
		printf("%d %d\n",user->iIndex,user->iKor);
	}
}