#pragma once
#include "TNodeBox.h"
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
		return g_iMaxUserCounter;
	}
	TNodeBox* GetHead()
	{
		return g_pHeadUserList;
	}
private:
	int    g_iMaxUserCounter;
	TNodeBox* g_pHeadUserList;
	TNodeBox* g_pEndUser;
public:
	/// <summary>
	/// 연결리스트 해당파일로 출력 기능
	/// </summary>
	bool FileSave();
public:
	TNodeBox* const NewNode();
	void DeleteAll();
	// 1)데이터 초기화 및 입력
	void Create();
	// 3) 로드해서 저장
	void Load();
	// 4) 화면 출력
	void Draw();
	
	void AddLink(TNodeBox* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};