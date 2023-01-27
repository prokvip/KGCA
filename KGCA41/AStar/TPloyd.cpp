#include "TPloyd.h"

namespace TPloyd
{
	// 1)2 차원 배열
	void TMapGenerator::Initialize()
	{
		m_dwNumNodes = m_WorldSize.x * m_WorldSize.y;		
		m_AllNodeList.resize(m_dwNumNodes);
		// 그래프 완성 및 2차원 배열 구축
		for (int y = 0; y < m_WorldSize.y; y++)
		{
			for (int x = 0; x < m_WorldSize.x; x++)
			{
				TIndex index(x, y);
				auto iter = std::find(m_Walls.begin(), m_Walls.end(), index);
				if (iter != m_Walls.end())
				{
					m_AllNodeList[y * m_WorldSize.x + x] = -1;
				}
				else
				{
					m_AllNodeList[y * m_WorldSize.x + x] = 1;
				}
			}
		}
		// 현재 노드에서 8방향 노드들만 직접 갈수 있다.
		for (int node = 0; node < m_dwNumNodes; node++)
		{
			std::vector<int> cost(m_dwNumNodes, 99999999);
			//cost.resize(m_dwNumNodes);
			int y = node / m_WorldSize.x;
			int x = node % m_WorldSize.x;
			cost[y * m_WorldSize.x + x] = 0;

			TIndex index(x, y);
			for (uint i = 0; i < 8; ++i)
			{
				TIndex  newIndex(index + m_Direction8[i]);
				if (!(newIndex.x < 0 || newIndex.x >= m_WorldSize.x ||
					newIndex.y < 0 || newIndex.y >= m_WorldSize.y))
				{
					if (m_AllNodeList[newIndex.y * m_WorldSize.x + newIndex.x] == 1)
					{
						if( i < 4)
							cost[newIndex.y * m_WorldSize.x + newIndex.x] = 10;
						else
							cost[newIndex.y * m_WorldSize.x + newIndex.x] = 14;
					}
				}
			}
			m_AllPathCostList.push_back(cost);
		}

	}
	// 2)삼중 순회-> 모든 노드 리스트 구축
	void TMapGenerator::AllPathGenerator()
	{
		m_dwNumPathList = m_dwNumNodes * m_dwNumNodes;
		//m_AllPathList.resize(m_dwNumPathList);

		for (int y = 0; y < m_dwNumNodes; y++)
		{
			RowPath rowpath;
			for (int x = 0; x < m_dwNumNodes; x++)
			{	
				pathvector path;
				int yValue = x / m_WorldSize.x;
				int xValue = x % m_WorldSize.x;
				TIndex index(xValue, yValue);
				path.push_back(index);	
				rowpath.push_back(path);
			}
			m_AllPathList.push_back(rowpath);
		}
		
		for (int i = 0; i < m_dwNumNodes; i++)
		{
			for (int j = 0; j < m_dwNumNodes; j++)
			{
				for (int k = 0; k < m_dwNumNodes; k++)
				{		
					if ((m_AllPathCostList[j][i] + m_AllPathCostList[i][k])
						<
						m_AllPathCostList[j][k])
					{
						// 경로를 바꾼다.
						m_AllPathCostList[j][k] = m_AllPathCostList[j][i] + 
												  m_AllPathCostList[i][k];
						m_AllPathList[j][k].assign(
							m_AllPathList[j][i].begin(),
							m_AllPathList[j][i].end());
						m_AllPathList[j][k].insert(
							m_AllPathList[j][k].end(),
							m_AllPathList[i][k].begin(),
							m_AllPathList[i][k].end());
					}
				}				
			}
		}
	}

	TMapGenerator::TMapGenerator()
	{
		m_Direction8.push_back({ 0,-1 });
		m_Direction8.push_back({ 0,+1 });
		m_Direction8.push_back({ -1,0 });
		m_Direction8.push_back({ +1,0 });

		m_Direction8.push_back({ -1,-1 });
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

	pathvector TMapGenerator::findpath(TIndex start,
		TIndex target)
	{
		int iBegin = start.y* m_WorldSize.x + start.x;
		int iEnd = target.y * m_WorldSize.x + target.x;
		pathvector ret = m_AllPathList[iBegin][iEnd];
		return ret;
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
	Node* TMapGenerator::findNodeList(NodeSet& list,
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
	uint TMapGenerator::Distance(TIndex s, TIndex t)
	{
		TIndex ret = { abs(s.x - t.x), abs(s.y - t.y) };
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