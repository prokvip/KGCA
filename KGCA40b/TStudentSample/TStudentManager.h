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
	TNodeBox<TStudent>* GetHead()
	{
		return g_pHeadUserList;
	}
private:
	int    g_iMaxUserCounter;
	TNodeBox<TStudent>* g_pHeadUserList;
	TNodeBox<TStudent>* g_pEndUser;
public:
	/// <summary>
	/// 연결리스트 해당파일로 출력 기능
	/// </summary>
	bool FileSave(const char* pFileName);
public:
	TNodeBox<TStudent>* const NewNode();
	void DeleteAll();
	// 1)데이터 초기화 및 입력
	void Create();
	// 3) 로드해서 저장
	void Load(const char* pFileName);
	// 4) 화면 출력
	void Draw();
	
	void AddLink(TNodeBox<TStudent>* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};