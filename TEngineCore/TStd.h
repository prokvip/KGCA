#pragma once
#include<windows.h>
#include < tchar.h >
#include <string>
#include <map>
#include <vector>
#include <list>
#pragma comment	(lib, "TEngineCore.lib")

extern float g_fSecPerFrame;
extern float g_fGameTimer;
extern HWND  g_hWnd;
extern RECT  g_rtClient;

template<class T>
class TSingleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

#define TBASIS_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
#define TBASIS_WIN(x) if( !gSample.InitWindow( hInstance, nCmdShow,  L#x ) )	{	return 0;	}	ShowCursor ( TRUE );gSample.Run();	return 1; }
#define TBASIS_RUN(x) TBASIS_START; TBASIS_WIN(x);