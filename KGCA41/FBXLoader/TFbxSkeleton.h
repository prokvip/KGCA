#pragma once
#include "TBasisStd.h"
#include <DirectXMath.h>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <string>
#include <fbxsdk.h>

#ifdef _DEBUG
#pragma comment	(lib, "libfbxsdk-md.lib")
#pragma comment	(lib, "libxml2-md.lib")
#pragma comment	(lib, "zlib-md.lib")
#else
#pragma comment	(lib, "libfbxsdk-mt.lib")
#pragma comment	(lib, "libxml2-mt.lib")
#pragma comment	(lib, "zlib-mt.lib")
#endif

// EXPORTSTRING_HASHSIZE must be a power of 2, so just change the shift value.
#define EXPORTSTRING_HASHSIZE (1 << 4)
//--------------------------------------------------------------------------------------
// namespace
//--------------------------------------------------------------------------------------
using namespace std;
using namespace DirectX;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class SkinData
{
public:
	static int				iNumMaxWeight;
	std::vector<FbxNode*>	InfluenceNodes;
	std::unordered_map<T_STR, TMatrix> m_matBindPoseMap;

	size_t					dwVertexCount;
	DWORD					dwVertexStride;
	std::unique_ptr<int[]>	pBoneIndices;
	std::unique_ptr<float[]> pBoneWeights;
public:
	void	Alloc(size_t dwCount, DWORD dwStride);
	int*	GetIndices(size_t dwIndex);
	float*	GetWeights(size_t dwIndex);
	DWORD	GetBoneCount() const;
	void	InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight);
public:
	SkinData() : dwVertexCount(0), dwVertexStride(0) {}
	~SkinData() {}
};
