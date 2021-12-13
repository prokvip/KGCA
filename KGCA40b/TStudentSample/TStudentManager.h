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
	/// ���Ḯ��Ʈ �ش����Ϸ� ��� ���
	/// </summary>
	bool FileSave();
public:
	TNodeBox* const NewNode();
	void DeleteAll();
	// 1)������ �ʱ�ȭ �� �Է�
	void Create();
	// 3) �ε��ؼ� ����
	void Load();
	// 4) ȭ�� ���
	void Draw();
	
	void AddLink(TNodeBox* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};