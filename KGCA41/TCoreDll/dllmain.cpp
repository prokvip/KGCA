// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "framework.h"
#include <iostream>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            int k=0;
            std::cout << k;
            
        }break;
        case DLL_THREAD_ATTACH:
        {
            int k = 0;
            std::cout << k;
           
        }break;
        case DLL_THREAD_DETACH:
        {
            int k = 0;
            std::cout << k;
        }break;
        case DLL_PROCESS_DETACH:
        {
            int k = 0;
            std::cout << k;           
        }
        break;
    }
    return TRUE;
}

