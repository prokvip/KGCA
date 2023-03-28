#pragma once
#define _DISABLE_EXTENDED_ALIGNED_STORAGE
#include <d3d11_1.h> 

#ifdef NO_TOOL_LIBRARY
	#include <dxgi1_2.h>
#endif

#include <dxgi.h>
#include <D3Dcompiler.h>
#include <dxgidebug.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include "TMath.h"

#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )

namespace TBASIS
{
	extern ID3D11Device*			g_pd3dDevice;
	extern ID3D11DeviceContext*		g_pImmediateContext;
}

/////////////////////////////////////////  반환하지 않는다. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//주의사항:매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행 되지 않게 x=DX메소드만 사용해야 한다.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

/////////////////////////////////////////  실폐의 경우만 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define H_RETURN(x){  hr = (x); if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
						}
#endif
#else
#define H_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; }}
#endif

/////////////////////////////////////////  무조건 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef U_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define U_RETURN(x){  hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
							else { return hr;}\
						}
#endif
#else
#define U_RETURN(x){ hr = (x); return hr; }
#endif


#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef T_RETURN
#define T_RETURN(x,ret){ hr = (x); if( FAILED(hr) ) { return ret; } }
#endif