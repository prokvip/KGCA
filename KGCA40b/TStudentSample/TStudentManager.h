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
	/// ���Ḯ��Ʈ �ش����Ϸ� ��� ���
	/// </summary>
	bool FileSave();
public:
	TStudent* const NewNode();
	void DeleteAll();
	// 1)������ �ʱ�ȭ �� �Է�
	void Create();
	// 2) ������ �߰�
	int FileSaveInsert(FILE* fp);
	// 3) �ε��ؼ� ����
	void Load();
	// 4) ȭ�� ���
	void Draw();
	
	void AddLink( TStudent* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};