#pragma once
//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)
#pragma warning( disable: 4251 )
#ifdef TBASISUI_EXPORTS
#define TBASISUI_API __declspec(dllexport)
#define TBASISUI_TEMPLATE
#else
#define TBASISUI_API __declspec(dllimport)
#define TBASISUI_TEMPLATE extern
#endif

#include <windows.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <codecvt>
#include <atlconv.h> 
#include <d3d11.h>
#include <dxgi1_3.h>
#include <d3dcompiler.h>
#include <assert.h>
#include "TVectorUI.h"
#include <wrl.h>//Windows Runtime C++ Template Library (WRL).

#ifdef _DEBUG
#include <dxgidebug.h>
#endif
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
using namespace Microsoft::WRL;


//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<DWORD>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<RECT>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<std::wstring>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<TTexture*>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<TSprite*>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::allocator<std::wstring>;
//TBASISUI_TEMPLATE template class TBASISUI_API std::shared_ptr<TSprite>;
//TBASISUI_TEMPLATE template struct TBASISUI_API std::pair<std::wstring,TSprite>;


namespace TUI
{
	class TBASISUI_API TTexture;
	class TBASISUI_API TSprite;
	class TBASISUI_API TWidget;
	class TBASISUI_API TBasisUI;

	extern HWND g_hWnd;
	extern RECT g_rtClient;
	extern float g_fGameTimer;
	extern float g_fSecondPerFrame;

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;
#define lerp(t,x,y)  (x+(t*(y - x)));
#define smoothstep(x, fmin, fmax) ((x<fmin) ? 0.0f: ( (x>fmax) ? 1.0f: x/fmax ));

#if defined(PROFILE) || defined(DEBUG)
	inline void DXUT_SetDebugName(ID3D11Resource* pObj, std::string text)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)text.size(), text.c_str());
	}
	inline void DXUT_SetDebugName(IDXGIObject* pObj, std::string text)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)text.size(), text.c_str());
	}
	inline void DXUT_SetDebugName(ID3D11Device* pObj, std::string text)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)text.size(), text.c_str());
	}
	inline void DXUT_SetDebugName(ID3D11DeviceChild* pObj, std::string text)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)text.size(), text.c_str());
	}
#else
#define DXUT_SetDebugName( pObj, pstrName )
#endif

	typedef std::basic_string<TCHAR> T_STR;
	typedef std::basic_string<wchar_t> W_STR;
	typedef std::basic_string<char>  C_STR;
	typedef std::vector<std::basic_string<TCHAR>>		TCHAR_STRING_VECTOR;
	typedef std::vector<DWORD>					DWORD_VECTOR;
	

	template<class T> class TSingleton
	{
	public:
		static T& GetInstance()
		{
			static T theSingleInstance;
			return theSingleInstance;
		}
	};
	//#include <atlconv.h> // A2W
	static std::wstring to_mw(const std::string& _src)
	{
		USES_CONVERSION;
		return std::wstring(A2W(_src.c_str()));
	};

	static std::string to_wm(const std::wstring& _src)
	{
		USES_CONVERSION;
		return std::string(W2A(_src.c_str()));
	};
	static std::wstring mtw(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(str);
	}
	//#include <codecvt>
	static std::string wtm(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(str);
	}
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  �ҽ�
			retData, iLength, // ���
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  �ҽ�
			pDest, iLength, // ���
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  �ҽ�
			retData, iLength); // ���
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  �ҽ�
			pDest, iLength); // ���		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // ����������
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}
};


