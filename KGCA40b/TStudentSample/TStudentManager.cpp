#include "TStudentManager.h"

 TStudent* const TStudentManager::NewNode()
{
	TStudent* pUser = new TStudent();
	pUser->m_iIndex = g_iMaxUserCounter;
	pUser->m_iKor = rand() % 100;
	pUser->m_iEng = rand() % 100;
	pUser->m_iMat = rand() % 100;
	pUser->m_iTotal =	pUser->m_iKor +
						pUser->m_iEng +
						pUser->m_iMat;
	pUser->m_fAverage = pUser->m_iTotal / 3.0f;
	pUser->m_pNext = NULL;
	return pUser;
}
void TStudentManager::AddLink( TStudent* const pUser)
{
	//pUser = 0; //error
	if (g_pHeadUserList == NULL)
	{
		g_pHeadUserList = pUser;
		g_pEndUser = pUser;
		g_iMaxUserCounter++;
		return;
	}
	g_pEndUser->m_pNext = pUser;
	g_pEndUser = pUser;
	g_iMaxUserCounter++;
}
bool TStudentManager::FileSave()
{
	FILE* fpWrite = fopen("test2.txt", "wb");
	// 블럭단위(덩어리) 입출력 함수
	int iCouner = g_iMaxUserCounter;
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (TStudent* user = g_pHeadUserList;
		user != NULL;
		user = user->m_pNext)
	{
		fwrite(user, sizeof(TStudent), 1, fpWrite);
	}
	fclose(fpWrite);
	return true;
}
void TStudentManager::Create()
{
	for (int iData = 0; iData < 1; iData++)
	{
		AddLink(NewNode());
	}
}
void TStudentManager::DeleteAll()
{
	TStudent* m_pNext = g_pHeadUserList;
	while (m_pNext)
	{
		TStudent* pDeleteUser = m_pNext;
		m_pNext = pDeleteUser->m_pNext;
		delete pDeleteUser;
		pDeleteUser = NULL;
		g_iMaxUserCounter--;
	}
	g_pHeadUserList = NULL;
}
int TStudentManager::FileSaveInsert(FILE* fp)
{
	TStudent* pSaveEndNode = g_pEndUser;
	for (int iData = 0; iData < 2; iData++)
	{
		AddLink(NewNode());
	}

	fwrite(&g_iMaxUserCounter, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (TStudent* user = pSaveEndNode->m_pNext;
		user != NULL;
		user = user->m_pNext)
	{
		fwrite(user, sizeof(TStudent), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return g_iMaxUserCounter;
}
void TStudentManager::Load()
{
	FILE* fpRead = fopen("test2.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int iAdd = 0; iAdd < iCounerRead; iAdd++)
		{
			TStudent* pUser = new TStudent(0, 0);
			memset(pUser, 0, sizeof(TStudent));
			fread(pUser, sizeof(TStudent), 1, fpRead);
			pUser->m_pNext = 0;
			AddLink(pUser);
		}
		fclose(fpRead);
		fpRead = NULL;
	}
}
void TStudentManager::Draw()
{	
	std::cout << *g_pHeadUserList;
}

std::ostream& operator << ( std::ostream& os, 
			const  TStudentManager& mgr)
{	
	for (TStudent* user = mgr.g_pHeadUserList;
		user != NULL;
		user = user->m_pNext)
	{
		std::cout << *user;
	}
	return os;
}