#pragma once
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