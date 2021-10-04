#include "TOctree.h"

void	TOctree::Frame(float time)
{
	//m_pPlayer->m_vForces.x = 10.0f;
	//m_pPlayer->m_vForces.y = 0.0f;
	//// F = MA, A =  F / M
	//m_pPlayer->m_vAcceleration.x = m_pPlayer->m_vForces.x / m_pPlayer->m_fMass;
	//m_pPlayer->m_vAcceleration.y = m_pPlayer->m_vForces.y / m_pPlayer->m_fMass;
	//m_pPlayer->m_vVelocity += m_pPlayer->m_vAcceleration * time;
	//m_pPlayer->m_vPos += m_pPlayer->m_vVelocity * time;
	//TNode* pFind = FindNode(m_pRootNode, m_pPlayer->m_vPos);
	//if (pFind != nullptr)
	//{
	//	cout << pFind->m_iIndex << " ";
	//}
	static float fDirection = 1.0f;
	if (m_pPlayer->m_vPos.x > 100.0f || m_pPlayer->m_vPos.x < 0.0f)
	{
		fDirection *= -1.0f;
	}
	m_pPlayer->m_vVelocity.x = 10.0f * fDirection;
	m_pPlayer->m_vVelocity.y = 0.0f;
	m_pPlayer->m_vPos += m_pPlayer->m_vVelocity * time;

	XVector3 vHalf = m_pPlayer->m_tRect.vSize / 2.0f;
	m_pPlayer->m_tRect.vPivot = { m_pPlayer->m_vPos.x - vHalf.x, m_pPlayer->m_vPos.y - vHalf.y, m_pPlayer->m_vPos.z + vHalf.z };
	m_pPlayer->m_tRect.vMin = { m_pPlayer->m_vPos.x - vHalf.x, m_pPlayer->m_vPos.y - vHalf.y, m_pPlayer->m_vPos.z - vHalf.z };
	m_pPlayer->m_tRect.vMax = { m_pPlayer->m_vPos.x + vHalf.x, m_pPlayer->m_vPos.y + vHalf.y, m_pPlayer->m_vPos.z + vHalf.z };

	TNode* pFind = FindNode(m_pRootNode, m_pPlayer);
	if (pFind != nullptr)
	{
		cout << pFind->m_iIndex << " ";
	}
}
bool    TOctree::Init(float fMaxX, float fMaxY, float fMaxZ)
{	
	m_pPlayer = new TObject();
	m_vSize = { fMaxX, fMaxY, fMaxZ };
	m_pRootNode = CreateNode(nullptr, XVector3(0, 0, 0), m_vSize);
	Buildtree(m_pRootNode);
	AddObject(m_pPlayer);
	return true;
}

void TOctree::Buildtree(TNode* pNode)
{
	if (pNode->m_tRect.vSize.x >= 30.0f &&  pNode->m_tRect.vSize.y >= 30.0f && pNode->m_tRect.vSize.z >= 30.0f)
	{
		XVector3  vHalf = pNode->m_tRect.vSize / 2.0f;
		pNode->m_pChild[0] = CreateNode(pNode, 	pNode->m_tRect.vPivot, vHalf);
		Buildtree(pNode->m_pChild[0]);
		
		pNode->m_pChild[1] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x + vHalf.x, pNode->m_tRect.vPivot.y, pNode->m_tRect.vPivot.z),vHalf);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x + vHalf.x, pNode->m_tRect.vPivot.y, pNode->m_tRect.vPivot.z-vHalf.z), vHalf);
		Buildtree(pNode->m_pChild[2]);
		
		pNode->m_pChild[3] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x, pNode->m_tRect.vPivot.y, pNode->m_tRect.vPivot.z - vHalf.z), vHalf);
		Buildtree(pNode->m_pChild[3]);		

		pNode->m_pChild[4] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x, pNode->m_tRect.vPivot.y+vHalf.y, pNode->m_tRect.vPivot.z), vHalf);
		Buildtree(pNode->m_pChild[4]);

		pNode->m_pChild[5] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x + vHalf.x, pNode->m_tRect.vPivot.y + vHalf.y, pNode->m_tRect.vPivot.z), vHalf);
		Buildtree(pNode->m_pChild[5]);

		pNode->m_pChild[6] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x + vHalf.x, pNode->m_tRect.vPivot.y + vHalf.y, pNode->m_tRect.vPivot.z - vHalf.z), vHalf);
		Buildtree(pNode->m_pChild[6]);

		pNode->m_pChild[7] = CreateNode(pNode, XVector3(pNode->m_tRect.vPivot.x, pNode->m_tRect.vPivot.y + vHalf.y, pNode->m_tRect.vPivot.z - vHalf.z), vHalf);
		Buildtree(pNode->m_pChild[7]);
	}
}
int TOctree::CheckRect(TNode* pNode, TObject* pObj)
{
	if (pNode->m_tRect.vMin.x <= pObj->m_tRect.vMin.x && pNode->m_tRect.vMax.x >= pObj->m_tRect.vMax.x)
	{
		if (pNode->m_tRect.vMin.y <= pObj->m_tRect.vMin.y && pNode->m_tRect.vMax.y >= pObj->m_tRect.vMax.y)
		{
			if (pNode->m_tRect.vMin.z <= pObj->m_tRect.vMin.z && pNode->m_tRect.vMax.z >= pObj->m_tRect.vMax.z)
			{
				return 1;
			}
		}
	}
	return 0;
}
TNode* TOctree::FindNode(TNode* pNode, TObject* pObj)
{
	do {
		for (int iNode = 0; iNode < 8; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr )//&&	pNode->m_pChild[iNode]->IsRect(pObj->m_vPos))				
			{
				if (CheckRect(pNode->m_pChild[iNode], pObj))
				{
					m_Queue.push(pNode->m_pChild[iNode]);
					break;
				}
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    TOctree::AddObject(TObject* pObj)
{
	TNode* pFindNode = FindNode(m_pRootNode, pObj);
	if (pFindNode != nullptr)
	{
		pObj->m_iNodeIndex = m_ObjectList.size();
		m_ObjectList.insert(make_pair(pObj->m_iNodeIndex, pObj));
		pFindNode->AddObject(pObj->m_iNodeIndex);
		return true;
	}
	return false;
}

void TOctree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
	for (auto& obj : m_ObjectList)
	{
		delete obj.second;
	 }
	m_ObjectList.clear();
}
TNode* TOctree::CreateNode(TNode* pParent, XVector3 vPivot, XVector3 vSize)
{
	TNode* pNode =	new TNode(vPivot, vSize);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}	
	pNode->m_iIndex = TNode::g_iNewCounter;	
	TNode::g_iNewCounter++;
	return pNode;
}
TOctree::TOctree()
{
	m_pRootNode = nullptr;
}

TOctree::~TOctree()
{
}
