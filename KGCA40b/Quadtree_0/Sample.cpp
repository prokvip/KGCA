#include "TQuadtree.h"
#include <Windows.h>
#pragma comment	(lib, "Winmm.lib")
void main()
{
	TQuadtree tree;
	tree.Init(100, 100, 2);
	// 정적오브젝트
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
	// 동적오브젝트
	TObject* ObjectList[10];
	for (int iObj = 0; iObj < 10; iObj++)
	{
		ObjectList[iObj] = new TObject();
		ObjectList[iObj]->SetPos((float)(rand() % 100),
			15);
		// -------w------
		// vMin
		//      c(pos)
		//               vMax
		float hw = (float)10;
		float hh = (float)2;
		TVector2 vMin;
		vMin.x = ObjectList[iObj]->m_vPos.x - hw;
		vMin.y = ObjectList[iObj]->m_vPos.y - hh;
		TRect rt(vMin, hw * 2.0f, hh * 2.0f);
		ObjectList[iObj]->SetRect(rt);
		tree.AddDynamicObject(ObjectList[iObj]);
	}
	float fTime = 0.01f;
	DWORD dwCurrentTime = timeGetTime();// tick(1000: 1초)
	DWORD dwTime=0;
	DWORD dwTimer = 0;
	static  float fGameTime= 25000.0f;
	while (fGameTime > 0.0f)
	{		
		tree.DynamicDeleteObject(tree.m_pRootNode);		
		for (int obj = 0; obj < 10; obj++)
		{
			TVector2 pos = ObjectList[obj]->m_vPos;
			pos = pos + TVector2(1, 0) * (dwTimer /1000.0f)*10;
			ObjectList[obj]->SetPos(pos.x, pos.y);			
			tree.AddDynamicObject(ObjectList[obj]);
		}	
		dwTime = timeGetTime();		
		dwTimer = dwTime - dwCurrentTime;
		fGameTime -= (dwTimer / 1000.0f);
		tree.PrintObjectList(tree.m_pRootNode);
		dwCurrentTime = dwTime;		
		Sleep(1000);
	}
	// 출력
	tree.PrintObjectList(tree.m_pRootNode);

	for (int i = 0; i < 10; i++)
	{
		delete ObjectList[i];
	}
}