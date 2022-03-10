#pragma once
#include "TStd.h"
#include <set>
#include <algorithm>

struct TIndexMap
{
	int m_X;
	int m_Y;
	bool    operator ==(const TIndexMap& p)
	{
		return (m_X == p.m_X && m_Y == p.m_Y);
	}
	TIndexMap    operator +(const TIndexMap& p)
	{
		return { m_X + p.m_X, m_Y + p.m_Y };
	}
	TIndexMap(int x, int y)
	{
		m_X = x;
		m_Y = y;
	}
	TIndexMap()
	{
		m_X = m_Y = 0;
	}
};
struct TNode
{
	TIndexMap	m_index;
	TNode*		m_pParent;
	DWORD		m_dwCost;
	DWORD		m_dwCostTotal;
	TNode(TIndexMap index, TNode* parent = nullptr)
	{
		m_index = index;
		m_pParent = parent;
		m_dwCost = 0;
		m_dwCostTotal = 0;
	}
};
class TAStar
{
	std::vector<TIndexMap>  m_FildPathList;
public:
	TIndexMap  m_WorldSize;
	std::vector<TIndexMap>  m_Walls;
	std::vector<TIndexMap>  m_Direction;
	std::set<TNode*>  OpenList;
	std::set<TNode*>  CloseList;
public:
	void		AddCollision(TIndexMap index);
	bool		FindPath(TIndexMap s, TIndexMap e);
	bool		DetectCollision(TIndexMap& index);
	TNode*		FindNodeOnList(std::set<TNode*>& list, TIndexMap& coord);
	TIndexMap    GetDelta(TIndexMap s, TIndexMap e);
	TAStar();
};

