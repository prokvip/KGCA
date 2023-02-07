#include "TCoreDll.h"
#include <iostream>
void main()
{
	int iValue = GerValue();
	std::cout << iValue;
	std::cout << g_iValue;

	TCoreDll core;
	std::cout << core.m_iValue;

}