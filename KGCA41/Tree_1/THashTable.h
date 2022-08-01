#pragma once
#include "TLinkedList.h"

const int g_iBucketSize = 100;

template < class K, class T>
class TBstNode
{
public:
	K			m_key;
	T			m_Data;
	int			m_iDepth;
	TBstNode*	m_pChild[2];
	TBstNode()
	{
		m_pChild[0] = nullptr;
		m_pChild[1] = nullptr;		
		m_iDepth = 0;
	}
	virtual ~TBstNode(){}
};

template < class K, class T>
class TBucket
{
public:
	K m_Key;
	T m_pData;
	int   hash(int iBucketSize)
	{
		return m_Key % iBucketSize;
	}
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
	int iLength;
	int   hash(int iBucketSize)
	{
		unsigned long hash = 5381;
		int c = 0;
		for (int i = 0; i < iLength; i++)
		{
			TCHAR value = m_Key[i];
			hash += value;
		}
		return hash % iBucketSize;
	}
	TBucket(char* k, T t)
	{
		iLength = strlen(k) + 1;
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
	//typedef TBucket<K, T> TBUCKET;
	using TBUCKET = TBucket<K, T>;
	TLinkedList<TBUCKET*>	m_Table;
	TLinkedList<TBUCKET*>	m_TableHash[g_iBucketSize];
	TBstNode<K, T>* m_pBstRoot;
	//int g_iBucketSize = 0;
	/*TTable(int iSize)
	{
		g_iBucketSize = iSize;
		m_TableHash = new TLinkedList<TBUCKET*>[iSize];
	}*/
public:
	int			hash(K key)// int
	{
		unsigned long hash = 0x45AB904C;
		unsigned long val = static_cast<unsigned long>(key);
		hash = (hash + val) << 5;
		return (hash ^ (hash >> 16)) % 10;
	}
	void		insert(K k, T t);
	T			find(K k);
	void		clear();
	T			bst(K key);
	int			size()
	{
		return m_Table.size();
	}
	void		erase(K k);
	TBstNode<K, T>* removeBst(K k, TBstNode<K, T>* t);
	void		CreateBinarySearchTree(TBstNode<K, T>* pBstNode, TBUCKET* pData);
	TBstNode<K, T>* RecursiveBST(TBstNode<K, T>* pNode, K key);
	TBstNode<K, T>* findMin(TBstNode<K, T>* t)
	{
		if (t == NULL)
			return NULL;
		else if (t->m_pChild[0] == NULL)
			return t;
		else
			return findMin(t->m_pChild[0]);
	}
	TBstNode<K, T>* findMax(TBstNode<K, T>* t) {
		if (t == NULL)
			return NULL;
		else if (t->m_pChild[1] == NULL)
			return t;
		else
			return findMax(t->m_pChild[1]);
	}
};

template < class K, class T>
T TTable<K, T>::find(K k)
{
	TBucket<K, T>  temp(k, nullptr);
	int hash = temp.hash(g_iBucketSize);
	for( TNode<TBUCKET*>* bucket = m_TableHash[hash].begin();
		bucket != m_TableHash[hash].end();
		bucket = m_TableHash[hash].next())
	{
		if (bucket->m_pData->m_Key == k)
		{
			TBUCKET* pDataBucket = bucket->m_pData;
			return pDataBucket->m_pData;
		}
	}
	return nullptr;
}
template < class K, class T>
T TTable<K, T>::bst(K key)
{
	TBstNode<K, T>* pFindNode = RecursiveBST(m_pBstRoot, key);
	if (pFindNode == nullptr) return nullptr;
	return pFindNode->m_Data;
}
template < class K, class T>
TBstNode<K, T>* TTable<K, T>::RecursiveBST(TBstNode<K, T>* pNode, K key)
{
	if (pNode == nullptr) return nullptr;
	if (pNode->m_key == key) return pNode;
	if (pNode->m_key >= key)
	{
		RecursiveBST(pNode->m_pChild[0], key);
	}
	else
	{
		RecursiveBST(pNode->m_pChild[1], key);
	}
}
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
	int hash = pNewBucket->hash(g_iBucketSize);
	m_TableHash[hash].push_back(pNewBucket);
	m_Table.push_back(pNewBucket);

	if (m_pBstRoot == nullptr)
	{
		m_pBstRoot = new TBstNode<K, T>;
		m_pBstRoot->m_key = k;
		m_pBstRoot->m_Data = pNewBucket->m_pData;
		return;
	}
	CreateBinarySearchTree(m_pBstRoot, pNewBucket);
}
template < class K, class T>
void TTable<K, T>::CreateBinarySearchTree(TBstNode<K, T>* pBstNode, TBUCKET* pData)
{
	// 루트 >= iValue ==> 왼쪽
	if (pBstNode->m_key >= pData->m_Key)
	{
		if (pBstNode->m_pChild[0] == nullptr)
		{
			pBstNode->m_pChild[0] = new TBstNode<K, T>();
			pBstNode->m_pChild[0]->m_key = pData->m_Key;
			pBstNode->m_pChild[0]->m_Data = pData->m_pData;
			pBstNode->m_pChild[0]->m_iDepth = pBstNode->m_iDepth + 1;
		}
		else
		{
			CreateBinarySearchTree(pBstNode->m_pChild[0], pData);
		}
	}
	// 루트 <  iValue ==> 오른쪽
	else
	{
		if (pBstNode->m_pChild[1] == nullptr)
		{
			pBstNode->m_pChild[1] = new TBstNode<K, T>();
			pBstNode->m_pChild[1]->m_key = pData->m_Key;
			pBstNode->m_pChild[1]->m_Data = pData->m_pData;
			pBstNode->m_pChild[1]->m_iDepth = pBstNode->m_iDepth + 1;
		}
		else
		{
			CreateBinarySearchTree(pBstNode->m_pChild[1], pData);
		}
	}
}

template < class K, class T>
void	TTable<K, T>::erase(K k)
{	
	m_pBstRoot = removeBst(k, m_pBstRoot);
}
template < class K, class T>
TBstNode<K, T>* TTable<K, T>::removeBst(K k, TBstNode<K, T>* t)
{
	TBstNode<K, T>* temp;
	if (t == NULL)
	{
		return NULL;
	}
	else if (k < t->m_key)
	{
		t->m_pChild[0] = removeBst(k, t->m_pChild[0]);
	}
	else if (k > t->m_key)
	{
		t->m_pChild[1] = removeBst(k, t->m_pChild[1]);
	}
	else if (t->m_pChild[0] && t->m_pChild[1])
	{
		temp = findMin(t->m_pChild[1]);
		t->m_key = temp->m_key;
		t->m_pChild[1] = removeBst(t->m_key, t->m_pChild[1]);
	}
	else
	{
		temp = t;
		if (t->m_pChild[0] == NULL)
			t = t->m_pChild[1];
		else if (t->m_pChild[1] == NULL)
			t = t->m_pChild[0];
		delete temp;
	}
	return t;
}