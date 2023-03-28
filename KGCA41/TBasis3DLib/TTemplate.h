#pragma once
#include "TBasisStd.h"
namespace TBASIS {

	template< class T > void Swap(T* a, T* b)
	{
		T Temp;
		Temp = *a;
		*a = *b;
		*b = Temp;
	}
	template<typename TContainer>
	void stl_wipe(TContainer& container)
	{
		typename TContainer::iterator iter;
		for (iter = container.begin(); iter != container.end(); ++iter)
		{
			delete *iter;
		}
		container.clear();
	}

	template<typename T>
	void stl_wipe_vector(vector<T>& rVector)
	{
		vector<T> emptyData;
		rVector.swap(emptyData);
	}

	////////////////////////////////////////////////////////
	// 해당 객체를 sdl::map으로 관리한다
	////////////////////////////////////////////////////////
	template< class Child >	class TTemplateMap
	{
	public:
		typedef unordered_map <int, Child*>			TemplateMap;
		typedef typename TemplateMap::iterator		TemplateMapItor;
		TemplateMapItor								TItor;
		TemplateMap									TMap;
		int											m_iCurIndex;
	public:
		// 초기화 한다.
		virtual bool		Init();
		// 인덱스를 통하여 객체를 리턴받는다.
		virtual Child*	const	GetPtr(int index);
		// 객체이름으로 객체를 리턴받는다.
		virtual Child* const	GetPtr(const TCHAR* szName);
		// 객체 포인터로 저장된 인덱스를 리턴받는다.
		virtual int			GetID(Child*);
		// 객체이름으로 저장된 인덱스를 리턴받는다.
		virtual int			GetID(const TCHAR* szName);
		// 전체 저장된 갯수를 얻는다.
		virtual int			Count();
		// 전체 저장된 객체를 삭제한다.
		virtual bool		Release();
		// 인덱스에 해당하는 객체를 삭제한다.
		virtual bool		Delete(int iIndex);
		// pPoint에 해당하는 객체를 삭제한다.
		virtual bool		Delete(Child *pPoint);
		virtual bool		Frame();
		virtual bool		Render(ID3D11DeviceContext*    pContext);

	public:
		TTemplateMap();
		virtual ~TTemplateMap();
	};

	template < class Child >
	int TTemplateMap< Child >::GetID(Child* pChild)
	{
		int iIndex = -1;
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (pChild == pPoint)
			{
				iIndex = (*itor).first;
				break;
			}
		}
		return iIndex;
	}
	template < class Child >
	int TTemplateMap< Child >::GetID(const TCHAR* szName)
	{
		int iIndex = -1;
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
			{
				iIndex = (*itor).first;
				break;
			}
		}
		return iIndex;
	}

	template < class Child >
	bool TTemplateMap< Child >::Init()
	{
		m_iCurIndex = 0;
		TMap.clear();
		return true;
	}
	template < class Child >
	int TTemplateMap< Child >::Count()
	{
		return (int)TMap.size();
	}

	template < class Child >
	bool TTemplateMap< Child >::Frame()
	{
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (pPoint)
				pPoint->Frame();
		}
		return true;
	}
	template < class Child >
	bool TTemplateMap< Child >::Render(ID3D11DeviceContext*    pContext)
	{
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (pPoint)
				pPoint->Render(pContext);
		}
		return true;
	}
	template < class Child >
	bool TTemplateMap< Child >::Release()
	{
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (pPoint)
				pPoint->Release();
			else
				return false;
			delete pPoint;
		}
		TMap.clear();
		m_iCurIndex = 0;
		return true;
	}
	template < class Child >
	Child* const TTemplateMap< Child >::GetPtr(int iIndex)
	{
		TemplateMapItor itor = TMap.find(iIndex);
		if (itor == TMap.end()) return NULL;
		Child *pPoint = (*itor).second;
		return pPoint;
	}
	template < class Child >
	 Child* const TTemplateMap< Child >::GetPtr(const TCHAR* szName)
	{
		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			Child *pPoint = (Child *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szName))
			{
				return pPoint;
			}
		}
		return NULL;
	}
	template < class Child >
	bool TTemplateMap< Child >::Delete(int iIndex)
	{
		Child* const pPoint = GetPtr(iIndex);
		if (pPoint)
		{
			pPoint->Release();
			TMap.erase(iIndex);
		}
		return true;
	}
	template < class Child >
	bool TTemplateMap< Child >::Delete(Child *pPoint)
	{
		if (pPoint)
		{
			pPoint->Release();
			TMap.erase(GetID(pPoint));
		}
		return true;
	}
	template < class Child >
	TTemplateMap< Child >::TTemplateMap()
	{
		m_iCurIndex = 0;
		TMap.clear();
	}

	template < class Child >
	TTemplateMap< Child >::~TTemplateMap()
	{
		Release();
	}
}