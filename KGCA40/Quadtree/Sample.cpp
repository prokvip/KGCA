// Quadtree.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "TQuadtree.h"
#include <windows.h>

int main()
{
    {
        TQuadtree g_Quadtree;
        g_Quadtree.Init(100, 100);
        //srand(time(NULL));
        for (int iObj = 0; iObj < 3; iObj++)
        {
            XVector2 pos;
            pos.x = (float)(rand() % 100);
            pos.y = (float)(rand() % 100);
            g_Quadtree.AddObject(pos);
        }

        float fSec = 0.0f;
        while (fSec < 10)
        {
            Sleep(10);// 1 /100
            // 경과시간
            g_Quadtree.Frame(10.0f / 1000.0f);                        
            fSec += 10.0f / 1000.0f;
        }
        TNode::g_iNewCounter--;
        g_Quadtree.Release();
    }
    cout << "Hello World!\n";
}
