#include "TQuadtree.h"
void main()
{
	TQuadtree tree;
	tree.Init(100, 100, 2);
	for (int iObj = 0; iObj < 10; iObj++)
	{
		TObject* pObj = new TObject();
		pObj->SetPos(	(float)(rand() % 100),
						(float)(rand() % 100));
		// -------w------
		// vMin
		//      c(pos)
		//               vMax
		float hw = (float)(rand() % 10) + 10;
		float hh = (float)(rand() % 10) + 10;
		TVector2 vMin;
		vMin.x = pObj->m_vPos.x - hw;
		vMin.y = pObj->m_vPos.y - hh;
		TRect rt(vMin,  hw*2.0f, hh*2.0f);
		pObj->SetRect(rt);
		tree.AddObject(pObj);
	}
	// Ãâ·Â
	tree.PrintObjectList(tree.m_pRootNode);
}