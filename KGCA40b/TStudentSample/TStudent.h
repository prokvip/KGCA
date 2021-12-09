#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
class TUser
{
public:
	int			iIndex;
	int			iKor;
	TUser*		pNext;
	TUser()
	{
		iIndex = 0;
		iKor = 0;
		pNext = NULL;
	}
	TUser(int i, int kor) 
	{
		iIndex = i;
		iKor = kor;
		pNext = NULL;
	}
	~TUser()
	{
	}
};
class TStudent
{
public:	
	TStudent()
	{
		g_iMaxUserCounter = 0;
		g_pHeadUserList = 0;
		g_pEndUser = 0;
	}
	~TStudent()
	{
		DeleteAll();
	}
	void SetUserCounter(int iCnt)
	{
		g_iMaxUserCounter = iCnt;
	}
	int  GetUserCounter()
	{
		return g_iMaxUserCounter;
	}
private:
	int    g_iMaxUserCounter;
	TUser* g_pHeadUserList;
	TUser* g_pEndUser;
public:
	/// <summary>
	/// 연결리스트 해당파일로 출력 기능
	/// </summary>
	bool FileSave();
public:
	TUser* NewNode();
	void DeleteAll();
	// 1)데이터 초기화 및 입력
	void Create();
	// 2) 데이터 추가
	int FileSaveInsert(FILE* fp);
	// 3) 로드해서 저장
	void Load();
	// 4) 화면 출력
	void Draw();
	void AddLink(TUser* pUser);
private:
};
