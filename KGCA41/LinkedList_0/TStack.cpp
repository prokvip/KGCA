#include "TStack.h"
//TStudent* TStack::begin() {
//	return TLinkedList::begin();
//};
//TStudent* TStack::end() {
//	return TLinkedList::end();
//};
//TStudent* TStack::next() {
//	return TLinkedList::next();
//};
//TStudent* TStack::Find(int index)
//{
//	return TLinkedList::Find(index);
//}
void    TStack::clear()
{
	TLinkedList::clear();
}
void	TStack::push(TStudent* pNewNode)
{
	push_back(pNewNode);
}
void    TStack::pop()
{
	erase(top());
}
TStudent* TStack::top()
{
	return begin();
}

TStack::TStack()
{
}
TStack::~TStack()
{
}