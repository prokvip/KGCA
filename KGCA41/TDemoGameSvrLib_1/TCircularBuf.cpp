#include "TCircularBuf.h"
#include <assert.h>

bool CircularBuffer::Peek(OUT char* destbuf, size_t bytes) const
{
	assert(mBuffer != nullptr);

	if (mARegionSize + mBRegionSize < bytes)
		return false;

	size_t cnt = bytes;
	size_t aRead = 0;

	/// A, B ���� �Ѵ� �����Ͱ� �ִ� ���� A���� �д´�
	if (mARegionSize > 0)
	{
		aRead = (cnt > mARegionSize) ? mARegionSize : cnt;
		memcpy(destbuf, mARegionPointer, aRead);
		cnt -= aRead;
	}

	/// �б� �䱸�� �����Ͱ� �� �ִٸ� B �������� �д´�
	if (cnt > 0 && mBRegionSize > 0)
	{
		assert(cnt <= mBRegionSize);

		/// ������ ���� �� �б�
		size_t bRead = cnt;

		memcpy(destbuf + aRead, mBRegionPointer, bRead);
		cnt -= bRead;
	}

	assert(cnt == 0);

	return true;

}

bool CircularBuffer::Read(OUT char* destbuf, size_t bytes)
{
	assert(mBuffer != nullptr);

	if (mARegionSize + mBRegionSize < bytes)
		return false;

	size_t cnt = bytes;
	size_t aRead = 0;


	/// A, B ���� �Ѵ� �����Ͱ� �ִ� ���� A���� �д´�
	if (mARegionSize > 0)
	{
		aRead = (cnt > mARegionSize) ? mARegionSize : cnt;
		memcpy(destbuf, mARegionPointer, aRead);
		mARegionSize -= aRead;
		mARegionPointer += aRead;
		cnt -= aRead;
	}

	/// �б� �䱸�� �����Ͱ� �� �ִٸ� B �������� �д´�
	if (cnt > 0 && mBRegionSize > 0)
	{
		assert(cnt <= mBRegionSize);

		/// ������ ���� �� �б�
		size_t bRead = cnt;

		memcpy(destbuf + aRead, mBRegionPointer, bRead);
		mBRegionSize -= bRead;
		mBRegionPointer += bRead;
		cnt -= bRead;
	}

	assert(cnt == 0);

	/// A ���۰� ����ٸ� B���۸� �� ������ ���� A ���۷� ���� 
	if (mARegionSize == 0)
	{
		if (mBRegionSize > 0)
		{
			if (mBRegionPointer != mBuffer)
				memmove(mBuffer, mBRegionPointer, mBRegionSize);

			mARegionPointer = mBuffer;
			mARegionSize = mBRegionSize;
			mBRegionPointer = nullptr;
			mBRegionSize = 0;
		}
		else
		{
			/// B�� �ƹ��͵� ���� ��� �׳� A�� ����ġ
			mBRegionPointer = nullptr;
			mBRegionSize = 0;
			mARegionPointer = mBuffer;
			mARegionSize = 0;
		}
	}

	return true;
}




bool CircularBuffer::Write(const char* data, size_t bytes)
{
	assert(mBuffer != nullptr);

	/// Read�� �ݴ�� B�� �ִٸ� B������ ���� ����
	if (mBRegionPointer != nullptr)
	{
		if (GetBFreeSpace() < bytes)
			return false;

		memcpy(mBRegionPointer + mBRegionSize, data, bytes);
		mBRegionSize += bytes;

		return true;
	}

	/// A�������� �ٸ� ������ �뷮�� �� Ŭ ��� �� ������ B�� �����ϰ� ���
	if (GetAFreeSpace() < GetSpaceBeforeA())
	{
		AllocateB();

		if (GetBFreeSpace() < bytes)
			return false;

		memcpy(mBRegionPointer + mBRegionSize, data, bytes);
		mBRegionSize += bytes;

		return true;
	}
	/// A������ �� ũ�� �翬�� A�� ����
	else
	{
		if (GetAFreeSpace() < bytes)
			return false;

		memcpy(mARegionPointer + mARegionSize, data, bytes);
		mARegionSize += bytes;

		return true;
	}
}



void CircularBuffer::Remove(size_t len)
{
	size_t cnt = len;

	/// Read�� ���������� A�� �ִٸ� A�������� ���� ����

	if (mARegionSize > 0)
	{
		size_t aRemove = (cnt > mARegionSize) ? mARegionSize : cnt;
		mARegionSize -= aRemove;
		mARegionPointer += aRemove;
		cnt -= aRemove;
	}

	// ������ �뷮�� �� ������� B���� ���� 
	if (cnt > 0 && mBRegionSize > 0)
	{
		size_t bRemove = (cnt > mBRegionSize) ? mBRegionSize : cnt;
		mBRegionSize -= bRemove;
		mBRegionPointer += bRemove;
		cnt -= bRemove;
	}

	/// A������ ������� B�� A�� ����ġ 
	if (mARegionSize == 0)
	{
		if (mBRegionSize > 0)
		{
			/// ������ ��� ���̱�
			if (mBRegionPointer != mBuffer)
				memmove(mBuffer, mBRegionPointer, mBRegionSize);

			mARegionPointer = mBuffer;
			mARegionSize = mBRegionSize;
			mBRegionPointer = nullptr;
			mBRegionSize = 0;
		}
		else
		{
			mBRegionPointer = nullptr;
			mBRegionSize = 0;
			mARegionPointer = mBuffer;
			mARegionSize = 0;
		}
	}
}


