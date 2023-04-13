

#include "TLog.h"
#include <windows.h>

int main()
{
    int iCnt = 0;
    while(iCnt++ < 100)
    {
        std::string msg = "0000001";
        msg += std::to_string(iCnt);
        TLog::Get().log(msg);
        Sleep(1);
    }
    std::cout << "End!\n";
}
