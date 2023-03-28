#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_WARNINGS
#ifndef UNICODE
#error "TBASIS는 유니코드로 컴파일 해야 합니다."
#endif
#pragma warning( disable:4005)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <tchar.H>
#include <time.h>
#include <crtdbg.h>
#include <cassert>
#include <math.h>
#include <set>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <bitset>
#include <limits>
#include <functional>
#include <unordered_map> // unordered_map

#include <codecvt>
#include <mmsystem.h> //timeGetTime
#include <wrl.h>//Windows Runtime C++ Template Library (WRL).
/////////////////    컴파일 주요 옵션 //////////////
// error C7510: 'Callback': 종속적 템플릿 일 때
// 1)C++ --> 언어 -->  준수모드 : 아니오 
// 2)C++ --> 일반 -->  경고를 오류로 처리  : 아니오
#include <memory>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace std;

#define BASIS_APP_KEY	TEXT("Software\\TBASIS11_SAMPLES")

extern std::wstring	g_szTBasisSamplePath;
extern std::wstring g_szDefaultDataPath;
//////////////////////////////////////////////
//  매크로
//////////////////////////////////////////////
//#define str(x) L#x
//#define xstr(x) str(x)
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif
//////////////////////////////////////////////
// Assert
//////////////////////////////////////////////
#ifndef Assert
	#if defined( _DEBUG ) || defined( _DEBUG )
		#define Assert(b) do {if (!(b)) {OutputDebugStringW(L"Assert: " #b L"\n");}} while(0)
	#else
		#define Assert(b)
	#endif //_DEBUG || _DEBUG
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// 디버그 메세지( 오류가 예상되는 곳에서 사용함 ) : 파일명과 라인 및 문자열이 출력되며 강제 종료됨.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define	DEBUGMSG(lpText)\
	{\
		TCHAR szBuffer[256];\
		_stprintf_s(szBuffer, _T("[File: %ws][Line: %d]\n[Note : %ws]"), _CRT_WIDE(__FILE__), __LINE__, lpText);	\
		MessageBox(NULL, szBuffer, _T("ERROR"), MB_ICONERROR);\
		_ASSERT(1);}			
#else
	#define DEBUGMSG(lpText)
#endif

//////////////////////////////////////////////
// WinMain 매크로
//////////////////////////////////////////////
#define TBASIS_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
#define TBASIS_WIN(x) if( !gSample.InitWindow( hInstance, nCmdShow,  L#x ) )	{	return 0;	}	ShowCursor ( TRUE );gSample.Run();	return 1; }
#define TBASIS_RUN(x) TBASIS_START; TBASIS_WIN(x);

#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)



//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
