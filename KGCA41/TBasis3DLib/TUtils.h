#pragma once
#include "TDefine.h"
#include <d3d11.h>
#include <dxgidebug.h>

namespace TBASIS
{
	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<T_STR>		T_STR_VECTOR;
	typedef basic_string<TCHAR>::iterator	T_ITOR;
	typedef basic_string<wchar_t>::iterator	W_ITOR;
	typedef basic_string<char>::iterator	C_ITOR;
	typedef vector<T_STR>		T_ARRAY_ITOR;
	typedef vector<DWORD>				DWORD_VECTOR;
	typedef	vector< DWORD >::iterator	DWORD_VECTOR_ITOR;
	typedef list<DWORD>					DWORD_LIST;
	typedef list<DWORD>::iterator		DWORD_LIST_ITOR;
	typedef list< HANDLE >				HANDLE_LIST;
	typedef	list< HANDLE >::iterator	HANDLE_LIST_ITOR;

#if defined(__d3d11_h__)
	typedef ID3D11Device			ID3DDevice;
	typedef ID3D11Buffer			ID3DBuffer;
	typedef ID3D11Texture3D 		ID3DVolumeTexture;
	typedef ID3D11VertexShader 		ID3DVertexShader;
	typedef ID3D11PixelShader 		ID3DPixelShader;
	typedef ID3D11RenderTargetView	ID3DRTView;
	typedef D3D11_CULL_MODE			D3D_CULL_MODE;
	typedef D3D11_USAGE				D3D_USAGE;
	typedef D3D11_FILL_MODE			D3D_FILE_MODE;
#endif
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// static
	///////////////////////////////////////////////////////////////////////////////////////////////////
	static void MemoryReporting()
	{
#if defined(DEBUG) | defined(_DEBUG)
		HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
		decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
		IDXGIDebug* debug;
		GetDebugInterface(IID_PPV_ARGS(&debug));
		OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
		debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
		OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
		debug->Release();
#endif
	}

	static const T_STR Res(T_STR szFileName)
	{		
		return g_szDefaultDataPath + szFileName;
	}
	// ��Ƽ����Ʈ �������� �����
	// ABCȫ�浿 = 9
	// [A][B][C] [0]
	// �����ڵ����Ʈ �������� �����
	// ABCȫ�浿 = 12
	// [A][0][B][0][C][0] [][][][][][]
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

	static void ErrorQuit(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}


	template<class T> class TSingleton
	{
	public:
		static T& GetInstance()
		{
			static T theSingleInstance;
			return theSingleInstance;
		}
	};
	/**
	@brief buffer structor

	Telnet���� ���۵Ǵ� �����Ϳ� ���� ���������� ó���ؾ� �ϱ� ���Ͽ�,
	ȿ�������� �����͸� �����ؾ� �� ����� ���� structor
	*/
	struct TBASIS_INPUT_MAP
	{
		bool bUpKey;
		bool bDownKey;
		bool bLeftKey;
		bool bRightKey;

		bool bWKey;
		bool bSKey;
		bool bAKey;
		bool bDKey;
		bool bQKey;
		bool bEKey;
		bool bZKey;
		bool bCKey;

		bool bLeftClick;
		bool bRightClick;
		bool bMiddleClick;

		bool bLeftHold;
		bool bRightHold;
		bool bMiddleHold;

		bool bExit;
		bool bSpace; // ī�޶��� �̵����ӵ��� ������Ų��.

		int  iMouseValue[3];

		bool bFullScreen;
		bool bChangeFillMode;
		bool bChangePrimitive;
		bool bChangeCullMode;
		bool bChangeCameraType;
		bool bDebugRender;
	};
	extern TBASIS_INPUT_MAP g_InputData;
}


