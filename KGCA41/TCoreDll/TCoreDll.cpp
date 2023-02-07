#include "TCoreDll.h"
TCoreDll::TCoreDll()
{
	m_iValue = 10;
}
//int g_iValue': ¿Á¡§¿«
extern TCOREDLL_API int g_iValue = 200;
TCOREDLL_API int GerValue()
{
	return 500;
}

