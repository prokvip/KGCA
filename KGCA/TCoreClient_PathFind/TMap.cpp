#include "TMap.h"

bool TMap::Init()
{
	m_Astar.m_WorldSize = { 8, 5 };
	m_Astar.AddCollision({ 1, 1});
	m_Astar.AddCollision({ 1, 2 });
	m_Astar.AddCollision({ 1, 3 });
	m_Astar.AddCollision({ 2, 3 });
	m_Astar.AddCollision({ 3, 3 });
	m_Astar.AddCollision({ 4, 0 });
	m_Astar.AddCollision({ 4, 1 });
	m_Astar.AddCollision({ 4, 2 });
	m_Astar.AddCollision({ 4, 3 });
	m_Astar.FindPath({2,2}, {6,2});
	return true;
}