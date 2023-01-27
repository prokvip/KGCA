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
		// ��� ����� �̵� ���ɿ���
		std::vector<int> m_AllNodeList;
		std::vector<std::vector<int>> m_AllPathCostList;
		std::vector<RowPath> m_AllPathList;
	public:
		void SetWorldSize(TIndex ws);
		void AddColision(TIndex ws);
		
		// 1)2 ���� �迭
		void Initialize();
		// 2)���� ��ȸ-> ��� ��� ����Ʈ ����
		void AllPathGenerator();

		pathvector findpath(TIndex start, TIndex end);
		bool DetectCollision(TIndex coord);
		Node* findNodeList(NodeSet& list, TIndex newIndex);
		uint Distance(TIndex src, TIndex target);
		void DeleteNodes(NodeSet& nodes);
		TMapGenerator();
	};
}

