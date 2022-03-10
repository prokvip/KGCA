#include "TAStar.h"
void		TAStar::AddCollision(TIndexMap index)
{
	m_Walls.push_back(index);
}
bool		TAStar::FindPath(TIndexMap s, TIndexMap target)
{
	std::vector<TIndexMap> currentList;
	OpenList.insert(new TNode(s, nullptr));
	// 1)오픈리스트에서 가장 가까운 노드를 선택한다.
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
		// 가장 가까운 노드 선택된다.
		if (pSelectNode->m_index == target)
		{
			break;
		}
		// 오픈리스트 중 선택된 노드
		currentList.push_back(pSelectNode->m_index);
		// 방문한 리스트에 추가
		CloseList.insert(pSelectNode);
		// 오픈리스트에 삭제
		OpenList.erase(
			std::find(OpenList.begin(),	
					  OpenList.end(), 
					  pSelectNode));
		//2) currentList의 주변 8방향 노드를 오픈리스트 추가한다.
		for (int i = 0; i < 8; i++)
		{
			TIndexMap coord(pSelectNode->m_index+m_Direction[i]);
			//2.1 충돌노드 제외
			if (DetectCollision(coord))
			{
				continue;
			}
			//2.2 방문한 노드리스트에 있으면 제외
			if (FindNodeOnList(CloseList, coord)!=nullptr)
			{
				continue;
			}
			
			/*int iCurrentCost = 10 * sqrt(pow(m_Direction[i].m_X, 2) +
								      pow(m_Direction[i].m_Y, 2));*/
			int iCurrentCost = (i < 4) ? 10 : 14;
			int iTotalCost = pSelectNode->m_dwCostTotal + iCurrentCost;
			//2.3 오픈리스트에 존재하면 제외
			TNode* pFindNode = FindNodeOnList(OpenList, coord);
			if (pFindNode== nullptr)
			{
				// 갈수 있는 노드이므로 오픈리스트에 추가
				pFindNode = new TNode(coord, pSelectNode);
				// 거리계산
				TIndexMap delta = GetDelta(pFindNode->m_index, target);
				// Tatget Distance->동서남북(10), 대각선(14)
				pFindNode->m_dwCost = 10*sqrt(pow(delta.m_X,2) + 
										      pow(delta.m_Y,2));
				// Start Distance 누적 비용
				pFindNode->m_dwCostTotal = iTotalCost;
				OpenList.insert(pFindNode);
			}
			else if(iTotalCost < pFindNode->m_dwCostTotal)
			{
				// 중요 : 부모노드를 변경할 수 있다.
				pFindNode->m_pParent = pSelectNode;
				pFindNode->m_dwCostTotal = iTotalCost;
			}			
		}
	}

	// 길찾기 성공하면
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
	// 맵 밖에 존재하면
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