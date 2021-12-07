#define _CRT_SECURE_NO_WARNINGS
#define MAX_USER 2
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
struct TUser
{
	int    iIndex;
	int    iKor;
	TUser* pNext;
};
struct TReturn
{
	FILE* fp;
	TUser* list;
};
int    g_iMaxUserCounter = 0;
TUser* g_pHeadUserList = 0; 
TUser* g_pEndUser = 0;

TUser* NewNode();
void DeleteAll();
// 1)데이터 초기화 및 입력
FILE* Create();
// 2) 데이터 추가
int FileSaveInsert(FILE* fp);
// 3) 로드해서 저장
void Load();
// 4) 화면 출력
void Draw(int iCounter);
void main(int argc, char* argv[])
{
	// 1)데이터 초기화 및 입력
	FILE* fp = Create();
	// 2)추가
	int iCounter = FileSaveInsert(fp);
	fclose(fp);
	DeleteAll();

	////// 3) 로드해서 저장
	Load();
	////// 3) 화면출력
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
		// 블럭단위(덩어리) 입출력 함수
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