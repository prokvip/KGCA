#include "TQuadtree.h"
int main()
{
    TQuadtree quadtree;
    quadtree.BuildQuadtree();
    std::cout << std::endl;
    //// P L R
    //quadtree.PreOrder(quadtree.GetRootNode()); std::cout << std::endl;
    //// L R P
    //quadtree.PostOrder(quadtree.GetRootNode()); std::cout << std::endl;
    //// Level Order
    //quadtree.LevelOrder(quadtree.GetRootNode());
    std::cout << "Hello World!\n";
}
