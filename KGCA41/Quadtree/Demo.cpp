#include <windows.h>
#include <iostream>
#include "TQuadtree.h"
int main()
{
    TQuadtree   qt;
    std::map<int, TObject*>  mapObjectlist;    
    if (qt.Create(TRect( -100,-100,200,200 )))
    {
        for (int iObj = 0; iObj < 10; iObj++)
        {           
            TObject* pNewObject = new TObject;
            TRect rt;
            rt.left = cosf(rand()) * 100.0f;
            rt.top = sinf(rand()) * 100.0f;
            rt.right = 10.0f;
            rt.bottom = 10.0f;
            pNewObject->Create(rt);                              

            mapObjectlist.insert(std::make_pair(iObj, pNewObject));
            qt.AddObject(pNewObject);
        }
    }
    while (1)
    {
        qt.Reset(qt.m_pRootNode);
        for (auto object : mapObjectlist)
        {
            TObject* pObject = object.second;
            pObject->SetPos(
                pObject->m_rt.left, 
                pObject->m_rt.top);
            qt.AddObject(pObject);
        }
        Sleep(1000);
    }
    for (auto object : mapObjectlist)
    {
        TObject* pObject = object.second;
        delete pObject;
    }
    mapObjectlist.clear();
    std::cout << "Hello World!\n";
}
