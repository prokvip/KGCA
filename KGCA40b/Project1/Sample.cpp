
#define _CRT_SECURE_NO_WARNINGS
#define MAX_USER 2
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
struct TUser
{
	int    iIndex;
	int    iKor;
};
struct TReturn
{
	FILE*  fp;
	TUser* list;
};
TUser userlist[MAX_USER];
//TUser* g_pUserList = 0; 
TUser* userlistRead = 0;
// 1)데이터 초기화 및 입력
FILE* Create();
// 2) 데이터 추가
int Insert(FILE* fp);
// 3) 로드해서 저장
void Load(int iCounter);
// 4) 화면 출력
void Draw(int iCounter);
void main(int argc, char* argv[])
{
	// 1)데이터 초기화 및 입력
	FILE* fp = Create();
	// 2)추가
	int iCounter = Insert(fp);
	//// 3) 로드해서 저장
	Load(iCounter);
	//// 3) 화면출력
	Draw(iCounter);
}
FILE*  Create()
{	
	userlist[0].iIndex = 2;
	userlist[0].iKor = 70;
	userlist[1].iIndex = 3;
	userlist[1].iKor = 60;

	FILE* fpWrite = fopen("test2.txt", "r+");
	if (fpWrite == NULL)
	{
		fpWrite = fopen("test2.txt", "w");
		// 블럭단위(덩어리) 입출력 함수
		int iCouner = MAX_USER;
		fwrite(&iCouner, sizeof(int), 1, fpWrite);
		fwrite(&userlist[0], sizeof(TUser), 2, fpWrite);
		fclose(fpWrite);
		fpWrite = fopen("test2.txt", "r+");
	}
	return fpWrite;
}
int Insert(FILE* fp)
{
	int iCounter = 2 + MAX_USER;
	//fseek(fpWrite, 0, SEEK_SET);
	fwrite(&iCounter, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	fwrite(&userlist[0], sizeof(TUser), 2, fp);
	fclose(fp);
	return iCounter;
}
void Load(int iCounter)
{	
	userlistRead = (TUser*)malloc(sizeof(TUser) * iCounter);
	FILE* fpRead = fopen("test2.txt", "r");
	int iCounerRead = 0;
	if (fpRead != NULL)
	{
		fread(&iCounerRead, sizeof(int), 1, fpRead);
		fread(userlistRead, sizeof(TUser), iCounerRead, fpRead);
		fclose(fpRead);
	}
}
void Draw(int iCnt)
{
	for (int iUser = 0;
		iUser < iCnt;
		iUser++)
	{
		printf("%d %d\n",
			userlistRead[iUser].iIndex,
			userlistRead[iUser].iKor);
	}
	free(userlistRead);
}