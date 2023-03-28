#include "TObjMgr.h"

int		TObjMgr::Load(ID3D11Device* pd3dDevice,	const TCHAR* strFileName,
	const TCHAR* strShaderName, bool bThread)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	// 종복 방지 
	if (strFileName)
	{

		_tsplitpath_s(strFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TActor *pPoint = (TActor *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}

	TActor* pNewModel = GetFileTypeID(strFileName);
	if (pNewModel == nullptr)
	{
		return -1;
	}

	pNewModel->Init();
	pNewModel->m_szName = szFileName;
	if (pNewModel->Load(pd3dDevice, strFileName, strShaderName, bThread))
	{		
		TMap.insert(make_pair(m_iCurIndex++, pNewModel));
		return m_iCurIndex - 1;
	}
	return -1;
};
int		TObjMgr::Add(TActor* pNode)
{
	TMap.insert(make_pair(m_iCurIndex++, pNode));	
	return m_iCurIndex - 1;
};


TActor* TObjMgr::GetFileTypeID(const TCHAR* pszFileName)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(pszFileName, Drive, Dir, FName, Ext);
	Ext[4] = 0;
	_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

	TActor* pModel = nullptr;
	if (!_tcsicmp(Ext, _T(".ASE")))
	{
		SAFE_NEW(pModel,TAseObj);
		pModel->m_ObjType = ASEFILE;
		return pModel;
	}
	if( !_tcsicmp( Ext, _T(".TBS") ) )
	{
		SAFE_NEW(pModel, TTbsObj);
		pModel->m_ObjType = TBSFILE;
		return pModel;
	}
	if( !_tcsicmp( Ext, _T(".SKM") ) )
	{
		SAFE_NEW(pModel, TSkinObj);
		pModel->m_ObjType = SKMFILE;
		return pModel;
	}
	if( !_tcsicmp( Ext, _T(".MAT") ) )
	{
		SAFE_NEW(pModel, TBoneObj);
		pModel->m_ObjType = MATFILE;
		return pModel;
	}
	return nullptr;
}

void TObjMgr::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	TTemplateMap< TActor >::TemplateMapItor itor;
	for (itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		TActor *pPoint = (TActor *)(*itor).second;
		if (pPoint)
		{
			pPoint->SetMatrix(pWorld, pView, pProj);
		}
	}
}

TObjMgr::TObjMgr()
{
}

TObjMgr::~TObjMgr()
{
}
