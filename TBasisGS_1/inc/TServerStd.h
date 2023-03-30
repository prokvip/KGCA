#ifndef TSERVER_STD
#define TSERVER_STD
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <Mstcpip.h>
#include <mswsock.h> // SO_UPDATE_ACCEPT_CONTEXT
#include <iostream>
#include <tchar.h>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <sstream>
using namespace std;

#pragma comment (lib, "ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib,"mswsock.lib")  // AcceptEx()

typedef basic_string<TCHAR>		TCHAR_STRING;
typedef basic_string<CHAR>		C_STR;

typedef std::vector<TCHAR_STRING> TCHAR_STRING_VECTOR;
template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};


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
		#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
	#endif

#endif 