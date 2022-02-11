#pragma once
#include <string>
#define		POOL_MAX_SIZE 4096
#define		POOL_SIZE_MASK (POOL_MAX_SIZE - 1)

template<class T>
class TObjectPool
{
	// volatile = > 컴파일러 최적화 방지, 변수의 캐쉬방지
	// 	static const int a = b=6;
	//  if(a) {} -> if(0) {}
	static void* volatile m_Pool[POOL_MAX_SIZE];
	static long long volatile g_iHead; // 생성 및 할당 인덱스
	static long long volatile g_iTail; // 소멸 및 해제 인덱스
	// 준비되어 있으면 준비된 오브젝트를 반환해주고
	// 준비된 오브젝트가 없으면 신규로 생성해서 반환해주자
	
public:
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			void* data = InterlockedExchangePointer(&m_Pool[i], nullptr);
			if (data != nullptr)
			{
				_aligned_free(m_Pool[i]);
			}
		}
	}
	// 메모리 변수->갱신->확인->캐쉬방지->확인
	static void* operator new(size_t size)
	{
		long long iCurrent = InterlockedIncrement64(&g_iHead)-1;//g_iHead++
		long long iPos = iCurrent & POOL_SIZE_MASK;
		void* pObj = InterlockedExchangePointer(&m_Pool[iPos], nullptr);
		if (pObj != nullptr) return pObj;
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void  operator delete(void* deleteObj)
	{
		int iCurrent = InterlockedIncrement64(&g_iTail) - 1;// g_iTail++;
		int iPos = iCurrent & POOL_SIZE_MASK;
		void* pObj = InterlockedExchangePointer(&m_Pool[iPos], deleteObj);
		if (pObj != nullptr)
		{
			_aligned_free(pObj);
		}
	}
};

template<typename T>
void* volatile TObjectPool<T>::m_Pool[POOL_MAX_SIZE] = {};
template<typename T>
long long  volatile TObjectPool<T>::g_iHead = 0; // 생성 및 할당 인덱스
template<typename T>
long long  volatile TObjectPool<T>::g_iTail = 0; // 소멸 및 해제 인덱스