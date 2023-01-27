#pragma once
#include "TPathStd.h"
namespace TPloyd
{
	using uint = unsigned int;
	using pathlist = std::list<TIndex>;
	using pathvector = std::vector<TIndex>;
	using RowPath = std::vector< pathvector>;

	struct Node
	{
		uint G, H;
		TIndex index;
		Node* parent;
		uint  GetScore()
		{
			return G + H;
		}
		Node(TIndex i, Node* parent = nullptr)
		{
			index = i;
			this->parent = parent;
			G = H = 0;
		}
	};
	using NodeSet = std::set<Node*>;

	class TMapGenerator
	{
		DWORD m_dwNumNodes;;
		DWORD m_dwNumPathList;;
	private:
		TIndex  m_WorldSize;
		uint    m_Direction;
		pathvector m_Direction8;
		pathvector m_Walls;
		// 모든 노드의 이동 가능여부
		std::vector<int> m_AllNodeList;
		std::vector<std::vector<int>> m_AllPathCostList;
		std::vector<RowPath> m_AllPathList;
	public:
		void SetWorldSize(TIndex ws);
		void AddColision(TIndex ws);
		
		// 1)2 차원 배열
		void Initialize();
		// 2)삼중 순회-> 모든 노드 리스트 구축
		void AllPathGenerator();

		pathvector findpath(TIndex start, TIndex end);
		bool DetectCollision(TIndex coord);
		Node* findNodeList(NodeSet& list, TIndex newIndex);
		uint Distance(TIndex src, TIndex target);
		void DeleteNodes(NodeSet& nodes);
		TMapGenerator();
	};
}

