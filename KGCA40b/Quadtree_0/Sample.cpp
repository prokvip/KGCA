#define _CRT_SECURE_NO_WARNINGS
#include "TQuadtree.h"
#include "TTimer.h"
void main()
{
	TTimer   g_Timer;
	g_Timer.Init();

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
			(float)(rand() % 100));
		// -------w------
		// vMin
		//      c(pos)
		//               vMax
		float hw = (float)5;
		float hh = (float)5;
		TVector2 vMin;
		vMin.x = ObjectList[iObj]->m_vPos.x - hw;
		vMin.y = ObjectList[iObj]->m_vPos.y - hh;
		TRect rt(vMin, hw * 2.0f, hh * 2.0f);
		ObjectList[iObj]->SetRect(rt);
		tree.AddDynamicObject(ObjectList[iObj]);
	}

	while (g_Timer.m_fTimer < 30.0f)
	{		
		g_Timer.Frame();
		tree.DynamicDeleteObject(tree.m_pRootNode);		
		for (int obj = 0; obj < 10; obj++)
		{
			ObjectList[obj]->Update(TVector2(1,0), 
						g_Timer.m_fSecondPerFrame);
			tree.AddDynamicObject(ObjectList[obj]);
		}	
		//TObject* pObj = ObjectList[0];
		//std::cout << "[" << pObj->m_iNodeIndex << "]" <<
		//	(int)pObj->m_vPos.x << ":" << (int)pObj->m_vPos.y << " " << std::endl;
		tree.PrintObjectList(tree.m_pRootNode);	
		char szBuffer[256] = { 0, };
		sprintf(szBuffer, "%10.4f\n", g_Timer.m_fTimer);
		OutputDebugStringA(szBuffer);
		Sleep(1000);
	}
	// 출력
	tree.PrintObjectList(tree.m_pRootNode);

	for (int i = 0; i < 10; i++)
	{
		delete ObjectList[i];
	}
}