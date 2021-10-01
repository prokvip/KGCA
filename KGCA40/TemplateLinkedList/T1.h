#pragma once
#include <iostream>

template < class T>
class Node
{
public:
	T	    m_Value;
	Node* m_pNext;
	int     Get();
};
template < class T>
int  Node<T>::Get()
{
	return m_Value;
}