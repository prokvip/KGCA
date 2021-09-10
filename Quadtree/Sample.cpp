// Quadtree.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "TQuadtree.h"


int main()
{
    {
        TQuadtree g_Quadtree;
        g_Quadtree.Init(100, 100);
        //srand(time(NULL));
        /*for (int iObj = 0; iObj < 3; iObj++)
        {
            TVector2 pos;
            pos.x = (float)(rand() % 100);
            pos.y = (float)(rand() % 100);
            g_Quadtree.AddObject(pos);
        }*/
        TNode::g_iNewCounter--;
        g_Quadtree.Release();
    }
    cout << "Hello World!\n";
}
