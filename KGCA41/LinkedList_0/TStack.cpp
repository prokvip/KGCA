#include "TStack.h"
template<class T>
T* TStack<T>::begin() {
	return TLinkedList::begin();
};
template<class T>
T* TStack<T>::end() {
	return TLinkedList::end();
};
template<class T>
T* TStack<T>::next() {
	return TLinkedList::next();
};
template<class T>
T* TStack<T>::Find(int index)
{
	return TLinkedList::Find(index);
}
template<class T>
void    TStack<T>::clear()
{
	TLinkedList::clear();
}
template<class T>
void	TStack<T>::push(T* pNewNode)
{
	push_back(pNewNode);
}
template<class T>
void    TStack<T>::pop()
{
	erase(top());
}
template<class T>
T* TStack<T>::top()
{
	return begin();
}
template<class T>
TStack<T>::TStack()
{
}
template<class T>
TStack<T>::~TStack()
{
}