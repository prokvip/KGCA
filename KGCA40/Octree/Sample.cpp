// Quadtree.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "TOctree.h"
#include <windows.h>

int main()
{
    TBox a(TVector3(-10.0f, -10.0f, -10.0f), TVector3(10.0f, 10.0f, 10.0f));
    TBox b(TVector3(-5.0f, -5.0f, -5.0f), TVector3(15.0f, 15.0f, 15.0f));
    TBox rtUnion = TCollision::UnionRegion(a, b);
    TBox rtIntersect;
    if (TCollision::IntersectRegion(a, b, &rtIntersect))
    {
        cout << "충돌!\n";
    }

    {
        TOctree g_Octree;
        g_Octree.Init(100, 100, 100);
        //srand(time(NULL));
        for (int iObj = 0; iObj < 30; iObj++)
        {
            TVector3 pos;
            pos.x = (float)(rand() % 100);
            pos.y = (float)(rand() % 100);
            pos.z = -(float)(rand() % 100);

            TVector3 rect;
            rect.x = (float)(rand() % 10) + 1.0f;
            rect.y = (float)(rand() % 10) + 1.0f;
            rect.z = (float)(rand() % 10) + 1.0f;

            g_Octree.AddObject(new TObject(pos, rect));
        }

        float fSec = 0.0f;
        while (fSec < 30)
        {
            Sleep(10);
            g_Octree.Frame(10.0f / 1000.0f);                        
            fSec += 10.0f / 1000.0f;
        }
        TNode::g_iNewCounter--;
        g_Octree.Release();
    }
    cout << "Hello World!\n";
}
