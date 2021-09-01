// MagicSquare.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
// 1)배열 생성
int   array[20][20];
int  InputData()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            array[i][j] = 0;
        }
    }
    // 초기화
    int iSize = 3;
    std::cout << "Select RowCount? : ";
    std::cin >> iSize;
    if (iSize < 0)  return -1;
    std::cout << "\n";
    return iSize;
}
void  Print(int iSize);
int main()
{
    while (1)
    {
        int iSize = InputData();
        if (iSize < 0) break;

        int iCount = 0;
        int iY = 0;
        int iX = iSize / 2;
        array[iY][iX] = ++iCount;

        // 작업
        for (int i = 1; i < iSize * iSize; i++)
        {
            iX++;
            iY--;
            if (iY < 0 && iX > iSize - 1)
            {
                iY += 2;
                iX--;
            }
            else
            {
                if (iY < 0)
                {
                    iY = iSize - 1;
                }

                if (iX > iSize - 1)
                {
                    iX = 0;
                }
            }
            if (array[iY][iX] != 0)
            {
                iY += 2;
                iX--;
            }
            ++iCount;
            array[iY][iX] = iCount;
        }
        // 출력
        Print(iSize);
    }

    std::cout << "End!\n";
}

void  Print(int iSize)
{   
    for (int i = 0; i < iSize; i++)
    {
        for (int j = 0; j < iSize; j++)
        {
            std::cout << array[i][j] << " ";
        }
        std::cout << "\n";
    }
}