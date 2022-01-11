#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <list>
#include <iostream>
#pragma comment	(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment	(lib, "TCoreNet_d.lib")
#else
#pragma comment	(lib, "TCoreNet_r.lib")
#endif
using namespace std;