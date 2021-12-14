#include "TStudentManager.h"

TNodeBox<TStudent>* const TStudentManager::NewNode()
{
	TNodeBox<TStudent>* pNodeBox = new TNodeBox<TStudent>();
	int iType = rand() % 3;
	TStudent* pNewObject = nullptr;
	if( iType == 0)
		pNewObject = new TStudentMiddle();
	else if (iType == 1)
		pNewObject = new TStudentHigh();
	else
		pNewObject = new TStudentCollege();
	
	pNewObject->SetData(m_List.m_iNumNode);
	pNodeBox->m_pData = pNewObject;
	return pNodeBox;
}
bool TStudentManager::FileSave(const  char* pFileName)
{
	if (m_FileIO.CreateFile(pFileName))
	{
		m_FileIO.Write(&m_List.m_iNumNode, sizeof(int));
		for (TNodeBox<TStudent>* pData = m_List.GetHead();
			pData != NULL;
			pData = pData->m_pNext)
		{
			pData->m_pData->Save();
			m_FileIO.Write(&pData->m_pData->m_iType,sizeof(int));
			m_FileIO.Write(&pData->m_pData->m_iBufferSize,sizeof(int));
			m_FileIO.Write(pData->m_pData->m_csBuffer, pData->m_pData->m_iBufferSize);
		}
		m_FileIO.CloseFile();
	}
	return true;
}
void TStudentManager::Create()
{
	for (int iData = 0; iData < 1; iData++)
	{
		m_List.AddLink(NewNode());
	}
}
void TStudentManager::DeleteAll()
{
	m_List.DeleteAll();
}

void TStudentManager::Load(const char* pFileName)
{
	if (m_FileIO.OpenFile(pFileName))
	{
		int iCounerRead = 0;
		m_FileIO.Read(&iCounerRead, sizeof(int));
		for (int iAdd = 0; iAdd < iCounerRead; iAdd++)
		{
			// error C2259: 'TStudent': 추상 클래스를 인스턴스화할 수 없습니다.
			TNodeBox<TStudent>* pData = new TNodeBox<TStudent>();
			int iType = -1;
			m_FileIO.Read(&iType, sizeof(int));
			
			if (iType == 1)
				pData->m_pData = new TStudentMiddle();
			else if (iType == 2)
				pData->m_pData = new TStudentHigh();
			else
				pData->m_pData = new TStudentCollege();

			m_FileIO.Read(&pData->m_pData->m_iBufferSize,
							sizeof(int));
			m_FileIO.Read(pData->m_pData->m_csBuffer,
						pData->m_pData->m_iBufferSize);

			pData->m_pData->Load();
			m_List.AddLink(pData);
		}
		m_FileIO.CloseFile();		
	}
}
void TStudentManager::Draw()
{	
	std::cout << *m_List.GetHead();
}

std::ostream& operator << ( std::ostream& os, 
			 TStudentManager& mgr)
{	
	for (TNodeBox<TStudent>* pData = mgr.m_List.GetHead();
		pData != NULL;
		pData = pData->m_pNext)
	{
		if (pData->m_pData != nullptr)
		{
			pData->m_pData->Show();
		}
	}
	return os;
}