#include <iostream>
#define MAX_SIZE 3
int  g_iSize = MAX_SIZE;
int  g_iArray[MAX_SIZE];
int  g_iTop;

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
        int iSelect=0;
        
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
    g_iTop = 0;
}
void Destroy() {

}
void Push(int iData)
{
    if (IsPull()==false)
    {
        g_iArray[g_iTop] = iData;
        g_iTop++;
    }    
}
int Pop() {
    if (IsEmpty()==false)
    {
        g_iTop--;
        return g_iArray[g_iTop];
    }
    return -1;
}
void Top() {
    std::cout << g_iArray[g_iTop-1] << "\n" << std::endl;
}
bool IsEmpty() {
    if( g_iTop == 0)
        return true;
    return false;
}
bool IsPull() {
    if (g_iTop == MAX_SIZE)
    {
        std::cout << "Is full!\n";
        return true;
    }
    return false;
}
void Print()
{
    for (int i = 0; i < g_iTop; i++)
    {
        std::cout << g_iArray[i] << " ";
    }
    std::cout << "\n";
}