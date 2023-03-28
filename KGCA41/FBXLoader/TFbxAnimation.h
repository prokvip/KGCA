#pragma once
#include "TFbxSkeleton.h"

static TMatrix DxConvertMatrix(TMatrix m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}

static XMFLOAT4X4 ConvertMatrix(const FbxMatrix& matrix)
{
	XMFLOAT4X4 matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}
static TMatrix ConvertMatrixA(const FbxMatrix& matrix)
{
	TMatrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
}

static void TransformXMatrix(XMFLOAT4X4* pDestMatrix, const XMFLOAT4X4* pSrcMatrix)
{
	XMFLOAT4X4 SrcMatrix;
	if (pSrcMatrix == pDestMatrix)
	{
		memcpy(&SrcMatrix, pSrcMatrix, sizeof(XMFLOAT4X4));
		pSrcMatrix = &SrcMatrix;
	}
	memcpy(pDestMatrix, pSrcMatrix, sizeof(XMFLOAT4X4));

	/*if (m_bFlipZ)
	{
	pDestMatrix->_13 = -pSrcMatrix->_13;
	pDestMatrix->_23 = -pSrcMatrix->_23;
	pDestMatrix->_43 = -pSrcMatrix->_43;

	pDestMatrix->_31 = -pSrcMatrix->_31;
	pDestMatrix->_32 = -pSrcMatrix->_32;
	pDestMatrix->_34 = -pSrcMatrix->_34;
	}*/

	// Apply the global unit scale to the translation components of the matrix.
	//pDestMatrix->_41 *= m_fUnitScale;
	//pDestMatrix->_42 *= m_fUnitScale;
	//pDestMatrix->_43 *= m_fUnitScale;
}
class ExportTransform
{
public:
	void SetIdentity();
	bool Initialize(DirectX::CXMMATRIX matrix);
	bool Initialize(DirectX::XMFLOAT4X4 matrix);
	const DirectX::XMFLOAT4X4& Matrix() const { return m_Matrix; }
	DirectX::XMFLOAT4X4& Matrix() { return m_Matrix; }
	void Multiply(const DirectX::XMFLOAT4X4& Matrix);
	void Normalize();
	const DirectX::XMFLOAT3& Position() const { return m_Position; }
	const DirectX::XMFLOAT4& Orientation() const { return m_Orientation; }
	const DirectX::XMFLOAT3& Scale() const { return m_Scale; }

protected:
	bool DecomposeMatrix();

protected:
	DirectX::XMFLOAT4X4 m_Matrix;
	DirectX::XMFLOAT3   m_Position;
	DirectX::XMFLOAT4   m_Orientation;
	DirectX::XMFLOAT3   m_Scale;
};

class ExportBase
{
public:
	ExportBase() : m_pDCCObject(nullptr) {}
	ExportBase(std::string name) : m_pDCCObject(nullptr)
	{
		m_Name = name;
	}
	virtual ~ExportBase();

	std::string GetName() const { return m_Name; }
	void SetName(std::string newName) { m_Name = newName; }

	void SetDCCObject(void* pDCCObject) { m_pDCCObject = pDCCObject; }
	void* GetDCCObject() const { return m_pDCCObject; }
protected:
	std::string         m_Name;
	void*               m_pDCCObject;
};


struct ExportAnimationPositionKey
{
	float               fTime;
	DirectX::XMFLOAT3   Position;
};
typedef std::vector<ExportAnimationPositionKey> PositionKeyList;

struct ExportAnimationOrientationKey
{
	float               fTime;
	DirectX::XMFLOAT4   Orientation;
};
typedef std::vector<ExportAnimationOrientationKey> OrientationKeyList;

struct ExportAnimationScaleKey
{
	float               fTime;
	DirectX::XMFLOAT3   Scale;
};
typedef std::vector<ExportAnimationScaleKey> ScaleKeyList;

class ExportFrame;

class ExportAnimationTransformTrack
{
public:
	ExportAnimationTransformTrack()
		: pSourceFrame(nullptr)
	{
	}
	void AddKey(float fTime, const ExportTransform& Transform);
	void AddKey(float fTime, const DirectX::XMFLOAT4X4& matTransform);
	void AddKey(float fTime, const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT4& Orientation, const DirectX::XMFLOAT3& Scale);
	void OptimizeKeys();
	void SortKeys();
	void EndianSwap();
	float* GetPositionData() const { return (float*)(PositionKeys.data()); }
	float* GetOrientationData() const { return (float*)(OrientationKeys.data()); }
	float* GetScaleData() const { return (float*)(ScaleKeys.data()); }
	size_t GetPositionDataSize() const { return PositionKeys.size() * 4 * sizeof(float); }
	size_t GetOrientationDataSize() const { return OrientationKeys.size() * 5 * sizeof(float); }
	size_t GetScaleDataSize() const { return ScaleKeys.size() * 4 * sizeof(float); }
	ExportAnimationPositionKey* GetPositionKeys() { return PositionKeys.data(); }
	size_t GetPositionKeyCount() const { return PositionKeys.size(); }
	ExportAnimationOrientationKey* GetOrientationKeys() { return OrientationKeys.data(); }
	size_t GetOrientationKeyCount() const { return OrientationKeys.size(); }
	ExportAnimationScaleKey* GetScaleKeys() { return ScaleKeys.data(); }
	size_t GetScaleKeyCount() const { return ScaleKeys.size(); }
	bool IsTrackEmpty();
protected:
	void OptimizePositionKeys();
	void OptimizeOrientationKeys();
	void OptimizeScaleKeys();

	bool PositionChangedFromLastTwoKeys(const ExportAnimationPositionKey& pk);
	bool OrientationChangedFromLastTwoKeys(const ExportAnimationOrientationKey& ok);
	bool ScaleChangedFromLastTwoKeys(const ExportAnimationScaleKey& sk);
public:
	PositionKeyList         PositionKeys;
	OrientationKeyList      OrientationKeys;
	ScaleKeyList            ScaleKeys;
	ExportFrame*            pSourceFrame;
};


class ExportAnimationTrack : public ExportBase
{
public:
	ExportAnimationTransformTrack       TransformTrack;
};

class ExportAnimation : public ExportBase
{
public:
	ExportAnimation();
	~ExportAnimation();
	void AddTrack(ExportAnimationTrack* pTrack) { m_vTracks.push_back(pTrack); }
	size_t GetTrackCount() const { return m_vTracks.size(); }
	ExportAnimationTrack* GetTrack(size_t dwIndex) { return m_vTracks[dwIndex]; }
	float GetDuration() const { return fEndTime - fStartTime; }
	void Optimize();
	void EndianSwap();
	static void SetAnimationExportQuality(INT iPos, INT iOrientation, INT iScale);
public:
	float                               fStartTime;
	float                               fEndTime;
	float                               fSourceSamplingInterval;
protected:
	std::vector< ExportAnimationTrack* >     m_vTracks;
};


class ExportFrame : public ExportBase
{
public:
	ExportFrame() {};
	ExportFrame(std::string name) {};
	virtual ~ExportFrame() {};

	void AddChild(ExportFrame* pFrame) { m_vChildren.push_back(pFrame); }
	size_t GetChildCount() const { return m_vChildren.size(); }
	ExportFrame* GetChildByIndex(size_t uIndex) { return m_vChildren[uIndex]; }
	ExportTransform& Transform() { return m_Transform; }

protected:
	ExportTransform                 m_Transform;
	std::vector< ExportFrame* >     m_vChildren;
};

