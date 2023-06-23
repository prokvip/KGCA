#include "TStudentManager.h"

int main()
{   
    TStudentManager mgr;
    mgr.Init();
    mgr.Run();
    printf("아무키나 누르시오!");
    _getche();
}

