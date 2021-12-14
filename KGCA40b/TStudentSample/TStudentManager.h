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
	/// ���Ḯ��Ʈ �ش����Ϸ� ��� ���
	/// </summary>
	bool FileSave(const char* pFileName);
public:
	TNodeBox<TStudent>* const NewNode();
	void DeleteAll();
	// 1)������ �ʱ�ȭ �� �Է�
	void Create();
	// 3) �ε��ؼ� ����
	void Load(const char* pFileName);
	// 4) ȭ�� ���
	void Draw();
	
	void AddLink(TNodeBox<TStudent>* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  TStudentManager& mgr);
private:
};