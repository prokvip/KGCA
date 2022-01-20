#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h> // inet_ntop
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <iostream>
#pragma comment	(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment	(lib, "TCoreServer_d.lib")
#else
#pragma comment	(lib, "TCoreServer_r.lib")
#endif
using namespace std;