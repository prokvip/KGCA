#pragma once
#include "framework.h"
class TCOREDLL_API TCoreDll
{
public:
	int m_iValue;
	TCoreDll();
};
extern "C"
{
	TCOREDLL_API TCoreDll* NewCore()
	{
		return new TCoreDll;
	}
}
TCOREDLL_API int GerValue();
TCOREDLL_API int g_iValue;

