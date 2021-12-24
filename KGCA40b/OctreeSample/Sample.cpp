#include "TOctree.h"
void main()
{
	TOctree tree;
	tree.Init(100, 100, 100, 2);
	for (int iObj = 0; iObj < 10; iObj++)
	{
		TObject* pObj = new TObject();
		pObj->SetPos(	(float)(rand() % 100),
						(float)(rand() % 100),
						(float)(rand() % 100));
		// -------w------
		// vMin
		//      c(pos)
		//               vMax
		float hw = (float)(rand() % 4) + 1;
		float hh = (float)(rand() % 4) + 1;
		float hz = (float)(rand() % 4) + 1;
		TVector3 vMin;
		vMin.x = pObj->m_vPos.x - hw;
		vMin.y = pObj->m_vPos.y - hh;
		vMin.z = pObj->m_vPos.z - hz;
		TBox rt(vMin,  hw*2.0f, hh*2.0f, hz * 2.0f);
		pObj->SetBox(rt);
		tree.AddObject(pObj);
	}
	// Ãâ·Â
	tree.PrintObjectList(tree.m_pRootNode);
}