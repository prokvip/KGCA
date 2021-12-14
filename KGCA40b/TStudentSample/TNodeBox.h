#pragma once
#include "TStudentMiddle.h"
#include "TStudentHigh.h"
#include "TStudentCollege.h"
template<class T>
class TNodeBox
{
public:
	T* m_pData;
	friend std::ostream& operator << (
		std::ostream& os,
		TNodeBox<T>& data)
	{
		os << *data.m_pData;
		return os;
	}
public:
	TNodeBox*	m_pNext;
	TNodeBox();
	TNodeBox(T* data);
	~TNodeBox();
};
//std::ostream& operator << (
//	std::ostream& os,
//	TNodeBox<T>& data)
//{
//	os << *data.m_pData;
//	return os;
//}
template<class T>
TNodeBox<T>::TNodeBox()
{
	m_pData = nullptr;
}
template<class T>
TNodeBox<T>::TNodeBox(T* data)
{
	m_pData = data;
}
template<class T>
TNodeBox<T>::~TNodeBox()
{
	delete m_pData;
}
