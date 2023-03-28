#ifndef TBASIS_H
#define TBASIS_H
#include "TUtils.h"
#include "TDxHelper.h"

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "runtimeobject.lib" )
#pragma comment( lib, "DbgHelp.lib" )

#if defined(DEBUG) || defined(_DEBUG)
	#if defined(_WIN64)
		#pragma comment( lib, "TBasis3D_D.lib" )
	#else
		#pragma comment( lib, "TBasis3D_D32.lib" )
	#endif
#else
	#if defined(_WIN64)
		#pragma comment( lib, "TBasis3D_R.lib" )
	#else
		#pragma comment( lib, "TBasis3D_R32.lib" )
	#endif
#endif

namespace TBASIS
{
	extern float		g_fSecPerFrame;
	extern float		g_fDurationTime;
	extern HWND			g_hWnd;
	extern HINSTANCE	g_hInstance;
	extern UINT			g_iClientWidth;
	extern UINT			g_iClientHeight;
	extern RECT			g_rtClient;
}
using namespace TBASIS;

#endif //TBASIS_H