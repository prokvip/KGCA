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
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 4096,// 2n승
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1,
	};
private:
	static void* m_mPool[POOL_MAX_SIZE];
	static long long m_HeadPos;
	static long long m_TailPos;
public:
	static void Allocation()
	{
		for (size_t i = 0; i < POOL_MAX_SIZE; i++)
		{
			// 메모리 시작 주소를 16바이트 정렬하겠다.
			// m_mPool[i] % MEMORY_ALLOCATION_ALIGNMENT = 0
			m_mPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
		}
		InterlockedAdd64(&m_TailPos, POOL_MAX_SIZE);
	}
	static void Release()
	{
		for (size_t i = 0; i < POOL_MAX_SIZE; i++)
		{
			_aligned_free(m_mPool[i]);
		}
	}
	//POOL_SIZE_MASK = 0111
	//m_HeadPos = 0,  0000 & 0111 = 0
	//m_HeadPos = 1,  0001 & 0111 = 1
	//m_HeadPos = 2,  0010 & 0111 = 2
	//m_HeadPos = 3,  0011 & 0111 = 3
	//m_HeadPos = 4,  0100 & 0111 = 4
	//m_HeadPos = 5,  0101 & 0111 = 5
	//m_HeadPos = 6,  0110 & 0111 = 6
	//m_HeadPos = 7,  0111 & 0111 = 7
	 
	//m_HeadPos = 8,  1000 & 0111 = 0
	//m_HeadPos = 9,  1001 & 0111 = 1
	//m_HeadPos = 10, 1010 & 0111 = 2
	static void* operator new (size_t size)
	{
		size_t pos = InterlockedIncrement64(&m_HeadPos)-1;
		size_t realpos = pos & POOL_SIZE_MASK;
		// ret=m_mPool[0] -> m_mPool[0]=null
		void* ret = InterlockedExchangePointer(&m_mPool[realpos], nullptr);
		if (ret != nullptr)
		{
			return ret;
		}
		return _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete (void* obj)
	{
		size_t pos = InterlockedIncrement64(&m_TailPos) - 1;
		size_t realpos = pos & POOL_SIZE_MASK;
		// ret=m_mPool[0] -> m_mPool[0]=null
		// 교체전 값 반환.
		void* ret = InterlockedExchangePointer(&m_mPool[realpos], obj);		
		if (ret != nullptr)
		{
			_aligned_free(ret);
		}
	}
};
template<typename T>
void* TObjectPool<T>::m_mPool[POOL_MAX_SIZE] = {};
template<typename T>
long long TObjectPool<T>::m_HeadPos(0);
template<typename T>
long long TObjectPool<T>::m_TailPos(0);