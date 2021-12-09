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
// 1)������ �ʱ�ȭ �� �Է�
FILE* Create();
// 2) ������ �߰�
int FileSaveInsert(FILE* fp);
// 3) �ε��ؼ� ����
void Load();
// 4) ȭ�� ���
void Draw(int iCounter);