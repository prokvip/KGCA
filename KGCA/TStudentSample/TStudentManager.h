#pragma once
#include "TLinkedList.cpp"
#include "TFileIO.h"
class TStudentManager
{
public:
	TStudentManager()
	{
	}
	~TStudentManager()
	{
		DeleteAll();
	}
public:
	TLinkedList<TStudent>  m_List;
	TFileIO  m_FileIO;
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

	friend std::ostream& operator << (
		std::ostream& os,
		  TStudentManager& mgr);
private:
};