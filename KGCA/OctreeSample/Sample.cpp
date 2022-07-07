#include "TOctree.h"
#include "TTimer.h"

#define MAX_OBJECT 10
void main()
{	
	TTimer g_Timer;
	g_Timer.Init();

	TOctree tree;
	tree.Init(100, 100, 100, 2);
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
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
	
	// 동적오브젝트
	TObject* ObjectList[10];
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		ObjectList[iObj] = new TObject();
		ObjectList[iObj]->SetPos((float)(rand() % 100),
			(float)(rand() % 100),
			(float)(rand() % 100));
		// -------w------
		// vMin
		//      c(pos)
		//               vMax
		float hw = (float)5;
		float hh = (float)5;
		float hq = (float)5;
		TVector3 vMin;
		vMin.x = ObjectList[iObj]->m_vPos.x - hw;
		vMin.y = ObjectList[iObj]->m_vPos.y - hh;
		vMin.z = ObjectList[iObj]->m_vPos.z - hq;
		TBox rt(vMin, hw * 2.0f, hh * 2.0f, hq*2.0f);
		ObjectList[iObj]->SetBox(rt);
		tree.AddDynamicObject(ObjectList[iObj]);
	}

	while (g_Timer.m_fTimer < 30.0f)
	{
		g_Timer.Frame();
		tree.DynamicDeleteObject(tree.m_pRootNode);
		for (int obj = 0; obj < MAX_OBJECT; obj++)
		{
			ObjectList[obj]->Update(TVector3(1, 1, 1 ),	g_Timer.m_fSecondPerFrame);
			tree.AddDynamicObject(ObjectList[obj]);
		}
		//TObject* pObj = ObjectList[0];
		//std::cout << "[" << pObj->m_iNodeIndex << "]" <<
		//	(int)pObj->m_vPos.x << ":" << (int)pObj->m_vPos.y << " " << std::endl;
		tree.PrintObjectList(tree.m_pRootNode);
		char szBuffer[256] = { 0, };
		sprintf_s(szBuffer, "%10.4f\n", g_Timer.m_fTimer);
		OutputDebugStringA(szBuffer);
		Sleep(1000);
	}
	// 출력
	tree.PrintObjectList(tree.m_pRootNode);

	for (int i = 0; i < MAX_OBJECT; i++)
	{
		delete ObjectList[i];
	}
	// 출력
	tree.PrintObjectList(tree.m_pRootNode);
}