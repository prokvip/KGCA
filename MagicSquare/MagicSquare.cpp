// MagicSquare.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
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
    const char* tokenstring = "15.0 12.3";
    float fp1;
    float fp2;
     sscanf(tokenstring, "%f%f", &fp1,&fp2);
    //FILE* fp = fopen("ObjectData.txt", "rt");
    //if (fp == NULL)
    //{
    //    return false;
    //}
    //char buffer[256] = { 0, };
    //int iNumVertex = 0;
    //fgets(buffer, 256, fp);
    //sscanf(buffer, ("%d"), &iNumVertex);

    //int index = 0;
    //for (int iLine = 0; iLine < iNumVertex; iLine++)
    //{
    //    float a=0;
    //    float b=0;
    //    fgets(buffer, 256, fp);
    //    sscanf(buffer, ("%f%f"), &a, &b);
    //    sscanf(buffer, ("%f%f"),
    //        &a, &b);
    //    /*_stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
    //        &index,
    //        &v.pos.x, &v.pos.y, &v.pos.z,
    //        &v.color.x, &v.color.y, &v.color.z, &v.color.w);*/
    //    //m_VertexList.push_back(v);
    //}
    //fclose(fp);
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