#include "TStudentManager.h"

TNodeBox* const TStudentManager::NewNode()
{
	TNodeBox* pUser = new TNodeBox(g_iMaxUserCounter);
	return pUser;
}
void TStudentManager::AddLink( TNodeBox* const pUser)
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
	FILE* fpWrite = fopen("test.txt", "wb");
	// ������(���) ����� �Լ�
	int iCouner = g_iMaxUserCounter;
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (TNodeBox* pData = g_pHeadUserList;
		pData != NULL;
		pData = pData->m_pNext)
	{
		pData->m_pData->Save();
		fwrite(&pData->m_pData->m_iType,
			sizeof(int), 1, fpWrite);
		fwrite(&pData->m_pData->m_iBufferSize, 
			sizeof(int), 1, fpWrite);
		fwrite(pData->m_pData->m_csBuffer,
			pData->m_pData->m_iBufferSize, 1, fpWrite);
	}
	fclose(fpWrite);
	return true;
}
void TStudentManager::Create()
{
	for (int iData = 0; iData < 5; iData++)
	{
		AddLink(NewNode());
	}
}
void TStudentManager::DeleteAll()
{
	TNodeBox* m_pNext = g_pHeadUserList;
	while (m_pNext)
	{
		TNodeBox* pDeleteUser = m_pNext;
		m_pNext = pDeleteUser->m_pNext;
		delete pDeleteUser;
		pDeleteUser = NULL;
		g_iMaxUserCounter--;
	}
	g_pHeadUserList = NULL;
}

void TStudentManager::Load()
{
	FILE* fpRead = fopen("test.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int iAdd = 0; iAdd < iCounerRead; iAdd++)
		{
			// error C2259: 'TStudent': �߻� Ŭ������ �ν��Ͻ�ȭ�� �� �����ϴ�.
			TNodeBox* pData = new TNodeBox();
			memset(pData, 0, sizeof(TNodeBox));
			int iType = -1;
			fread(&iType, sizeof(int), 1, fpRead);
			if (iType == 1)
				pData->m_pData = new TStudentMiddle();
			else if (iType == 2)
				pData->m_pData = new TStudentHigh();
			else
				pData->m_pData = new TStudentCollege();						
			
			fread(&pData->m_pData->m_iBufferSize,
				sizeof(int), 1, fpRead);
			fread(pData->m_pData->m_csBuffer,
				pData->m_pData->m_iBufferSize, 1, fpRead);

			pData->m_pData->Load();
			AddLink(pData);
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
	for (TNodeBox* pData = mgr.g_pHeadUserList;
		pData != NULL;
		pData = pData->m_pNext)
	{
		//std::cout << *pData;
		pData->m_pData->Show();
	}
	return os;
}