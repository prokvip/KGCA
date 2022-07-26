#pragma once
#include "TLinkedList.h"
template < class K, class T>
class TBucket
{
public:
	K m_Key;
	T m_pData;
	TBucket(K k, T t)
	{
		m_Key = k;
		m_pData = t;
	}
	~TBucket() {
	}
};
template <class T>
class TBucket<char*,T>
{
public:
	char* m_Key;
	T m_pData;
	TBucket(char* k, T t)
	{
		m_Key = new char[strlen(k)+1];
		memcpy(m_Key, k, sizeof(char) * strlen(k) + 1);
		m_pData = t;
	}
	~TBucket() {
		delete m_Key;
		m_Key = nullptr;
	}
};
template < class K, class T>
class TTable
{
public:
	typedef TBucket<K, T> TBUCKET;
	//using TBUCKET = TBucket<K, T>;
	TLinkedList<TBUCKET*> m_Table;
public:
	void		insert(K k, T t);
	void		clear();
	int			size()
	{
		return m_Table.size();
	}
};
template < class K, class T>
void	TTable<K, T>::clear()
{
	//using DATA2 = TBucket<char*, TItem*>*;
	//using TNODE2 = TNode<DATA2>*;
	if (!m_Table.empty())
	{
		for (TNode<TBUCKET*>* pNode = m_Table.begin();
			 pNode != m_Table.end();
			pNode = m_Table.next())
		{
			TBUCKET* pData = pNode->m_pData;
			if (pData) delete pData;
		}
		m_Table.clear();
	}
}
template < class K, class T>
void	TTable<K,T>::insert(K k, T t)
{
	TBucket<K, T>* pNewBucket = new TBucket<K, T>(k, t);
	m_Table.push_back(pNewBucket);
}