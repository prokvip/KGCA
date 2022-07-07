#include "TAStar.h"
void		TAStar::AddCollision(TIndexMap index)
{
	m_Walls.push_back(index);
}
bool		TAStar::FindPath(TIndexMap s, TIndexMap target)
{
	std::vector<TIndexMap> currentList;
	OpenList.insert(new TNode(s, nullptr));
	// 1)���¸���Ʈ���� ���� ����� ��带 �����Ѵ�.
	TNode* pSelectNode = nullptr;
	while (!OpenList.empty())
	{
		pSelectNode = *OpenList.begin();
		for (auto node : OpenList)
		{
			int iNodeCost = node->m_dwCost;
			int iCurCost = pSelectNode->m_dwCost;
			if (iNodeCost <= iCurCost)
			{
				pSelectNode = node;
			}
		}
		// ���� ����� ��� ���õȴ�.
		if (pSelectNode->m_index == target)
		{
			break;
		}
		// ���¸���Ʈ �� ���õ� ���
		currentList.push_back(pSelectNode->m_index);
		// �湮�� ����Ʈ�� �߰�
		CloseList.insert(pSelectNode);
		// ���¸���Ʈ�� ����
		OpenList.erase(
			std::find(OpenList.begin(),	
					  OpenList.end(), 
					  pSelectNode));
		//2) currentList�� �ֺ� 8���� ��带 ���¸���Ʈ �߰��Ѵ�.
		for (int i = 0; i < 8; i++)
		{
			TIndexMap coord(pSelectNode->m_index+m_Direction[i]);
			//2.1 �浹��� ����
			if (DetectCollision(coord))
			{
				continue;
			}
			//2.2 �湮�� ��帮��Ʈ�� ������ ����
			if (FindNodeOnList(CloseList, coord)!=nullptr)
			{
				continue;
			}
			
			/*int iCurrentCost = 10 * sqrt(pow(m_Direction[i].m_X, 2) +
								      pow(m_Direction[i].m_Y, 2));*/
			int iCurrentCost = (i < 4) ? 10 : 14;
			int iTotalCost = pSelectNode->m_dwCostTotal + iCurrentCost;
			//2.3 ���¸���Ʈ�� �����ϸ� ����
			TNode* pFindNode = FindNodeOnList(OpenList, coord);
			if (pFindNode== nullptr)
			{
				// ���� �ִ� ����̹Ƿ� ���¸���Ʈ�� �߰�
				pFindNode = new TNode(coord, pSelectNode);
				// �Ÿ����
				TIndexMap delta = GetDelta(pFindNode->m_index, target);
				// Tatget Distance->��������(10), �밢��(14)
				pFindNode->m_dwCost = 10*sqrt(pow(delta.m_X,2) + 
										      pow(delta.m_Y,2));
				// Start Distance ���� ���
				pFindNode->m_dwCostTotal = iTotalCost;
				OpenList.insert(pFindNode);
			}
			else if(iTotalCost < pFindNode->m_dwCostTotal)
			{
				// �߿� : �θ��带 ������ �� �ִ�.
				pFindNode->m_pParent = pSelectNode;
				pFindNode->m_dwCostTotal = iTotalCost;
			}			
		}
	}

	// ��ã�� �����ϸ�
	while (pSelectNode != nullptr)
	{
		m_FildPathList.push_back(pSelectNode->m_index);
		pSelectNode = pSelectNode->m_pParent;
	}
	if (m_FildPathList.size() <= 0)
	{
		return false;
	}
	std::reverse(m_FildPathList.begin(),m_FildPathList.end());
	for (auto it = OpenList.begin(); it != OpenList.end();)
	{
		delete* it;
		it = OpenList.erase(it);
	}

	for (auto it = CloseList.begin(); it != CloseList.end();)
	{
		delete* it;
		it = CloseList.erase(it);
	}
	return true;
}
bool		TAStar::DetectCollision(TIndexMap& index)
{
	// �� �ۿ� �����ϸ�
	if (index.m_X < 0 || index.m_X >= m_WorldSize.m_X ||
		index.m_Y < 0 || index.m_Y >= m_WorldSize.m_Y)
	{
		return true;
	}
	auto iter = std::find(m_Walls.begin(), m_Walls.end(), index);
	if (iter == m_Walls.end())
	{
		return false;
	}
	return true;
}
TNode* TAStar::FindNodeOnList(std::set<TNode*>& list, TIndexMap& coord)
{
	for (auto node : list)
	{
		if (node->m_index == coord)
		{
			return node;
		}
	}
	return nullptr;
}
TIndexMap    TAStar::GetDelta(TIndexMap s, TIndexMap e)
{
	TIndexMap ret;
	ret.m_X = abs(s.m_X - e.m_X);
	ret.m_Y = abs(s.m_Y - e.m_Y);
	return ret;
}
TAStar::TAStar()
{
	m_Direction.resize(8);
	// -1,-1    0,-1   1,-1
	// -1,0     0,0    1, 0 
	// -1,1     0,1    1,1
	m_Direction[0] = { 0, - 1 };
	m_Direction[1] = { -1, 0 };
	m_Direction[2] = { 1, 0 };
	m_Direction[3] = { 0, 1 };

	m_Direction[4] = { -1, -1 };
	m_Direction[5] = { 1, -1 };
	m_Direction[6] = { -1, 1 };
	m_Direction[7] = { 1, 1 };
}