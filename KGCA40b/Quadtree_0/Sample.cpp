#include "TQuadtree.h"
void main()
{
	TQuadtree tree;
	tree.Init(100, 100, 2);
	for (int iObj = 0; iObj < 10; iObj++)
	{
		int iX = rand() % 100;
		int iY = rand() % 100;
		tree.AddObject(iX, iY);
	}
	// Ãâ·Â
	tree.PrintObjectList(tree.m_pRootNode);
}