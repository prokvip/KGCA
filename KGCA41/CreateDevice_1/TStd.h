#pragma once
#include <windows.h>
#include <string>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>			//D3DComplileFromFile
#pragma comment (lib, "dxgi.lib") // d3d11.dll
#pragma comment (lib, "d3d11.lib") // d3d11.dll
#pragma comment (lib, "d3dcompiler.lib") // d3d11.dll

#define TGAME_START int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int       nCmdShow){ Sample win;
#define TGAME_REGISTER      win.SetRegisterClassWindow(hInstance);
#define TGAME_WINDOW(s,w,h) win.SetWindow(L#s, w, h);
#define TGAME_RUN win.Run();
#define TGAME_END return 0; };

#define TGAME(s,w,h) TGAME_START;TGAME_REGISTER;TGAME_WINDOW(s,w,h);TGAME_RUN;TGAME_END;