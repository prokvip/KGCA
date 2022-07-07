#pragma once
#include <string>
#define		POOL_MAX_SIZE 4096
#define		POOL_SIZE_MASK (POOL_MAX_SIZE - 1)

template<class T>
class TObjectPool
{
	// volatile = > �����Ϸ� ����ȭ ����, ������ ĳ������
	// 	static const int a = b=6;
	//  if(a) {} -> if(0) {}
	static void* volatile m_Pool[POOL_MAX_SIZE];
	static long long volatile g_iHead; // ���� �� �Ҵ� �ε���
	static long long volatile g_iTail; // �Ҹ� �� ���� �ε���
	// �غ�Ǿ� ������ �غ�� ������Ʈ�� ��ȯ���ְ�
	// �غ�� ������Ʈ�� ������ �űԷ� �����ؼ� ��ȯ������
	
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
	// �޸� ����->����->Ȯ��->ĳ������->Ȯ��
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
long long  volatile TObjectPool<T>::g_iHead = 0; // ���� �� �Ҵ� �ε���
template<typename T>
long long  volatile TObjectPool<T>::g_iTail = 0; // �Ҹ� �� ���� �ε���