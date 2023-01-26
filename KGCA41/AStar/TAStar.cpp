#include "TAStar.h"
namespace AStar
{
	TMapGenerator::TMapGenerator()
	{
		m_Direction8.push_back({ 0,-1 });
		m_Direction8.push_back({ 0,+1 });
		m_Direction8.push_back({ -1,0 });
		m_Direction8.push_back({ +1,0 });

		m_Direction8.push_back({-1,-1});		
		m_Direction8.push_back({ +1,-1 });
		m_Direction8.push_back({ -1,+1 });		
		m_Direction8.push_back({ +1,+1 });		
	}
	void TMapGenerator::SetWorldSize(TIndex ws)
	{
		m_WorldSize = ws;
	}
	void TMapGenerator::AddColision(TIndex index)
	{
		m_Walls.push_back(index);
	}

	pathlist TMapGenerator::findpath(	TIndex start,
										TIndex target)
	{
		NodeSet openSet, closedSet;
		openSet.insert(new Node(start));
		std::vector<TIndex>  currentList;
		Node* current = nullptr;
		while (!openSet.empty())
		{
			current = *openSet.begin();
			for (auto node : openSet)
			{
				// 비용처리
				uint iNodeCost = node->GetScore();
				uint iCurrentCost = current->GetScore();
				if (iNodeCost <= iCurrentCost)
				{
					current = node;
				}
			}
			// 목적지 도착
			if (current->index == target)
			{
				break;
			}
			// 방문했으면
			currentList.push_back(current->index);
			closedSet.insert(current);
			openSet.erase(current);// std::find(openSet.begin(), openSet.end(), current));
			for (uint i = 0; i < 8; ++i)
			{
				TIndex  newIndex(current->index + m_Direction8[i]);
				// 벽이야
				if (DetectCollision(newIndex))
				{
					continue;
				}
				// 이미 방문했으면
				if (findNodeList(closedSet,newIndex))
				{
					continue;
				}
				uint totalCost = current->G + ((i < 4) ? 10 : 14);// 이외는;
				Node* node = findNodeList(openSet, newIndex);
				if (node == nullptr)
				{
					node = new Node(newIndex, current);
					node->G = totalCost;
					node->H = Distance(node->index, target);
					openSet.insert(node);
				}
				else if( totalCost < node->G)
				{
					// 오픈리스트에 있다면
					node->parent = current;
					node->G = totalCost;
				}				
			}
		}
		pathlist path;
		while (current != nullptr)
		{
			path.push_front(current->index);
			current = current->parent;
		}
		DeleteNodes(openSet);
		DeleteNodes(closedSet);
		return path;		
	}

	bool TMapGenerator::DetectCollision(TIndex coord)
	{
		if (coord.x < 0 || coord.x >= m_WorldSize.x ||
			coord.y < 0 || coord.y >= m_WorldSize.y)
		{
			return true;
		}

		if (std::find(m_Walls.begin(),
					  m_Walls.end(), coord) 
					!= m_Walls.end())
		{
			return true;
		}		
		return false;
	}
	Node* TMapGenerator::findNodeList(  NodeSet& list, 
										TIndex newIndex)
	{
		for (auto node : list)
		{
			if (node->index == newIndex)
			{
				return node;
			}
		}
		return nullptr;
	}
	uint TMapGenerator::Distance(TIndex s,TIndex t)
	{
		TIndex ret = { abs(s.x - t.x), abs(s.y - t.y)};
		//return 10 * (ret.x + ret.y);
		return sqrt(pow(ret.x, 2) + pow(ret.y, 2)) * 10;
	}
	void TMapGenerator::DeleteNodes(NodeSet& nodes)
	{
		for (auto it = nodes.begin();
			it != nodes.end();
			)
		{
			delete* it;
			it = nodes.erase(it);
		}
	}
};