#include "TFbxSkeleton.h"
int SkinData::iNumMaxWeight = 0;
void SkinData::Alloc(size_t dwCount, DWORD dwStride)
{
	dwVertexCount = dwCount;
	dwVertexStride = dwStride;

	size_t dwBufferSize = dwVertexCount * dwVertexStride;
	pBoneIndices.reset(new int[dwBufferSize]);
	ZeroMemory(pBoneIndices.get(), sizeof(float) * dwBufferSize);

	pBoneWeights.reset(new float[dwBufferSize]);
	ZeroMemory(pBoneWeights.get(), sizeof(float) * dwBufferSize);
}


int* SkinData::GetIndices(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneIndices.get() + (dwIndex * dwVertexStride);
}
float* SkinData::GetWeights(size_t dwIndex)
{
	assert(dwIndex < dwVertexCount);
	return pBoneWeights.get() + (dwIndex * dwVertexStride);
}

DWORD SkinData::GetBoneCount() const 
{ 
	return static_cast<DWORD>(InfluenceNodes.size()); 
}

void SkinData::InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight)
{
	assert(dwBoneIndex < 256);

	auto pIndices = GetIndices(dwIndex);
	auto pWeights = GetWeights(dwIndex);

	for (DWORD i = 0; i < dwVertexStride; ++i)
	{
		if (fBoneWeight > pWeights[i])
		{
			for (DWORD j = (dwVertexStride - 1); j > i; --j)
			{
				pIndices[j] = pIndices[j - 1];
				pWeights[j] = pWeights[j - 1];
			}
			pIndices[i] = static_cast<int>(dwBoneIndex);
			pWeights[i] = fBoneWeight;
			break;
		}
		// 최대 본 인덱스를 얻는다.
		if (iNumMaxWeight < i)
		{
			iNumMaxWeight = i;
		}
	}
}