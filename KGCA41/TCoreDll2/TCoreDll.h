#pragma once
#include "framework.h"
#define _cplusplus

class TCOREDLL_API TCoreDll
{
public:
	int m_iValue;
	TCoreDll();
};

#ifdef _cplusplus
extern "C"
{
	TCOREDLL_API int GerValue();
	TCOREDLL_API int g_iValue;
}
#endif

