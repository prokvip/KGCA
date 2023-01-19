#pragma once
#include <queue>
#include <stdexcept>
#include <memory>
using std::shared_ptr;

template<typename T>
class TClassPool
{
public:
	TClassPool(size_t chunkSize = DefalultChunkSize);
	shared_ptr<T> NewChunk();
	void DeleteChunk(shared_ptr<T> obj);
protected:	
	size_t m_ChunkSize;
	static const size_t DefalultChunkSize = 10;
	std::queue<shared_ptr<T>> m_List;
	void AllocateChunk();
//private:
//	TClassPool(const TClassPool < T)& chunk);
//	TClassPool<T>& operator = (const TClassPool<T>& chunk);
};
template<typename T>
TClassPool<T>::TClassPool(size_t chunkSize )
{
	m_ChunkSize = chunkSize;
	AllocateChunk();
}
template<typename T>
void TClassPool<T>::AllocateChunk()
{
	for (size_t i = 0; i < m_ChunkSize; i++)
	{
		m_List.push(std::make_shared<T>());
	}
}
template<typename T>
shared_ptr<T> TClassPool<T>::NewChunk()
{
	if (m_List.empty())
	{
		m_List.push(std::make_shared<T>());
		m_ChunkSize++;
	}
	auto chunk = m_List.front();
	m_List.pop();
	return chunk;
}
template<typename T>
void TClassPool<T>::DeleteChunk(shared_ptr<T> obj)
{
	m_List.push(obj);
}


template<typename T>
class TObjectTool
{

};