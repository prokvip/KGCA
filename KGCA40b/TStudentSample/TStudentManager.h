#pragma once
#include "TStudent.h"
class TStudentManager
{
public:
	TStudentManager()
	{
		g_iMaxUserCounter = 0;
		g_pHeadUserList = 0;
		g_pEndUser = 0;
	}
	~TStudentManager()
	{
		DeleteAll();
	}
	void SetUserCounter(int iCnt)
	{
		g_iMaxUserCounter = iCnt;
	}	
	int  GetUserCounter() const
	{
		//SetUserCounter(3); // error
		//g_iMaxUserCounter = 0; // error
		return g_iMaxUserCounter;
	}
	TStudent* GetHead()
	{
		return g_pHeadUserList;
	}
private:
	int    g_iMaxUserCounter;
	TStudent* g_pHeadUserList;
	TStudent* g_pEndUser;
public:
	/// <summary>
	/// 연결리스트 해당파일로 출력 기능
	/// </summary>
	bool FileSave();
public:
	TStudent* const NewNode();
	void DeleteAll();
	// 1)데이터 초기화 및 입력
	void Create();
	// 2) 데이터 추가
	int FileSaveInsert(FILE* fp);
	// 3) 로드해서 저장
	void Load();
	// 4) 화면 출력
	void Draw();
	
	void AddLink( TStudent* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};