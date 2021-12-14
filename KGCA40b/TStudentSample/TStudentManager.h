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

	friend std::ostream& operator << (
		std::ostream& os,
		  TStudentManager& mgr);
private:
};