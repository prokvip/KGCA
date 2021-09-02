#include <iostream>
#define MAX_SIZE 10
int  g_iSize = MAX_SIZE;
int  g_iArray[MAX_SIZE];
int  g_iBack;
int  g_iFront;

void Create();
void Destroy();
void Push(int iData);
int  Pop();
void Top();
void Print();
bool IsEmpty();
bool IsPull();
int main()
{
    Create();
    bool bRun = true;
    while (bRun)
    {
        int iSelect = 0;

        std::cout << "0:Push,1:Top,2:Pop,3:Print,4:Exit\n";
        std::cout << "Select? : ";
        std::cin >> iSelect;
        switch (iSelect)
        {
        case 0:
        {
            int iData;
            std::cin >> iData;
            Push(iData);
            system("cls");
        }break;
        case 1:
        {
            Top();
        }break;
        case 2:
        {
            int iData = Pop();
            if (iData >= 0)
            {
                std::cout << iData << "End!\n";
            }
            else
            {
                std::cout << "Empty!\n";
            }
        }break;
        case 3:
        {
            Print();
        }break;
        default:bRun = false;
            break;
        }

    }
    Destroy();
    std::cout << "End!\n";
}
void Create()
{
    memset(g_iArray, 0, sizeof(int) * 10);
    g_iBack = 0;
    g_iFront = 0;
}
void Destroy() {

}
void Push(int iData)
{
    if (IsPull() == false)
    {
        g_iArray[g_iBack] = iData;
        g_iBack = (g_iBack+1) % MAX_SIZE;             
    }
}
int Pop() {
    if (IsEmpty() == false)
    {          
        int iRet = g_iArray[g_iFront];
        g_iFront = (g_iFront + 1) % MAX_SIZE;
        return iRet;
    }
    return -1;
}
void Top() {
    std::cout << g_iArray[g_iBack - 1] << "\n" << std::endl;
}
bool IsEmpty() {
    return g_iFront == g_iBack;
}
bool IsPull() {
    bool bCheck = 
        g_iFront ==(g_iBack + 1) % MAX_SIZE;
    if (bCheck)
    {
        std::cout << "Is full!\n";
        return true;
    }
    return false;
}
void Print()
{
    for (int i = 0; i < g_iBack; i++)
    {
        std::cout << g_iArray[i] << " ";
    }
    std::cout << "\n";
}