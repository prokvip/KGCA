#include "TQuadtree.h"
int main()
{    
    TQuadtree quadtree;
    quadtree.BuildQuadtree();
   
    std::vector<TObject*> objectlist;
    for (int i = 0; i < 100; i++)
    {
        TObject* obj = new TObject();
        obj->m_csName = L"obj";
        obj->m_csName += std::to_wstring(i);
        obj->m_Position = { (float)(rand() % 800), (float)(rand() % 600) };
        quadtree.AddObject(obj);

        objectlist.push_back(obj);
    }


    std::cout << std::endl;
    std::cout << "Object inform!\n";
    quadtree.LevelOrder(quadtree.GetRootNode());
    std::cout << "Hello World!\n";

    for (int i = 0; i < objectlist.size(); i++)
    {
        delete objectlist[i];
    }
    objectlist.clear();
    
}
