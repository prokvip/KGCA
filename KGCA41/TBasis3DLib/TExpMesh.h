#pragma once
#include "TObjStd.h"
#include <tchar.h>

enum T_VERTEX_USAGE
{
	T_USAGE_POSITION = 0,
	T_USAGE_BLENDWEIGHT = 1,
	T_USAGE_BLENDINDICES = 2,
	T_USAGE_NORMAL = 3,
	T_USAGE_TEXCOORD = 5,
	T_USAGE_TANGENT = 6,
	T_USAGE_BINORMAL = 7,
	T_USAGE_COLOR = 10,
};

enum T_DECL_TYPE
{
	T_TYPE_FLOAT1 = 0,  // 1D float expanded to (value, 0., 0., 1.)
	T_TYPE_FLOAT2 = 1,  // 2D float expanded to (value, value, 0., 1.)
	T_TYPE_FLOAT3 = 2,  // 3D float expanded to (value, value, value, 1.)
	T_TYPE_FLOAT4 = 3,  // 4D float
	T_TYPE_D3DCOLOR = 4,  // 4D packed unsigned bytes mapped to 0. to 1. range
							   // Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
							   T_TYPE_UBYTE4 = 5,  // 4D unsigned byte
							   T_TYPE_UBYTE4N = 8,  // Each of 4 bytes is normalized by dividing to 255.0
							   T_TYPE_SHORT4N = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
														  // Note: There is no equivalent to D3DDECLTYPE_DEC3N (14) as a DXGI_FORMAT
														  T_TYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
														  T_TYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values

														  T_TYPE_UNUSED = 17,  // When the type field in a decl is unused.

																					// These are extensions for DXGI-based versions of Direct3D
																					T_TYPE_DXGI_R10G10B10A2_UNORM = 32 + DXGI_FORMAT_R10G10B10A2_UNORM,
																					T_TYPE_DXGI_R11G11B10_FLOAT = 32 + DXGI_FORMAT_R11G11B10_FLOAT,
																					T_TYPE_DXGI_R8G8B8A8_SNORM = 32 + DXGI_FORMAT_R8G8B8A8_SNORM,
																					T_TYPE_XBOX_R10G10B10_SNORM_A2_UNORM = 32 + 189,
};

#pragma pack(push,4)

struct T_VERTEX_ELEMENT
{
	WORD    Stream;     // Stream index
	WORD    Offset;     // Offset in the stream in bytes
	BYTE    Type;       // Data type
	BYTE    Method;     // Processing method
	BYTE    Usage;      // Semantics
	BYTE    UsageIndex; // Semantic index
};

#pragma pack(pop)


struct TVertexFormat
{
public:
	TVertexFormat()
		: m_bPosition(true),
		m_bNormal(true),
		m_bSkinData(false),
		m_bTangent(false),
		m_bBinormal(false),
		m_bVertexColor(true),
		m_uUVSetCount(1),
		m_uUVSetSize(2)
	{
	}
	bool        m_bPosition;
	bool        m_bNormal;
	bool        m_bTangent;
	bool        m_bBinormal;
	bool        m_bSkinData;
	bool        m_bVertexColor;
	UINT        m_uUVSetCount;
	UINT        m_uUVSetSize;
};

struct ExportMeshVertex
{
public:
	ExportMeshVertex()
	{
		Initialize();
	}
	void Initialize()
	{
		ZeroMemory(this, sizeof(ExportMeshVertex));
		BoneWeights.x = 1.0f;
	}
	UINT                      DCCVertexIndex;
	TVector3               Position;
	TVector3               Normal;
	TVector3               SmoothNormal;
	TVector3               Tangent;
	TVector3               Binormal;
	TVector4				  BoneIndices;
	TVector4               BoneWeights;
	TVector4               TexCoords[8];
	TVector4              Color;
	ExportMeshVertex*               pNextDuplicateVertex;

	bool Equals(const ExportMeshVertex* pOtherVertex) const;
};

typedef std::vector< ExportMeshVertex* > ExportMeshVertexArray;

class ExportVB
{
public:
	ExportVB()
		: m_uVertexCount(0),
		m_uVertexSizeBytes(0)
	{
	}
	~ExportVB()
	{
	}

	void SetVertexSize(DWORD uByteCount) { m_uVertexSizeBytes = uByteCount; }
	DWORD GetVertexSize() const { return m_uVertexSizeBytes; }

	void SetVertexCount(size_t uVertexCount) { m_uVertexCount = uVertexCount; }
	size_t GetVertexCount() const { return m_uVertexCount; }

	void Allocate();

	uint8_t* GetVertex(size_t uIndex);
	const uint8_t* GetVertex(size_t uIndex) const;

	uint8_t* GetVertexData() { return m_pVertexData.get(); }
	const uint8_t* GetVertexData() const { return m_pVertexData.get(); }
	size_t GetVertexDataSize() const { return m_uVertexSizeBytes * m_uVertexCount; }

	void ByteSwap(const T_VERTEX_ELEMENT* pVertexElements, const size_t dwVertexElementCount);

protected:
	DWORD                       m_uVertexSizeBytes;
	size_t                      m_uVertexCount;
	std::unique_ptr<uint8_t[]>  m_pVertexData;
};

class ExportIB
{
public:
	ExportIB()
		: m_uIndexCount(0),
		m_dwIndexSize(2)
	{
	}
	~ExportIB()
	{
	}

	void SetIndexSize(DWORD dwIndexSize) { assert(dwIndexSize == 2 || dwIndexSize == 4); m_dwIndexSize = dwIndexSize; }
	DWORD GetIndexSize() const { return m_dwIndexSize; }

	void SetIndexCount(size_t uIndexCount) { m_uIndexCount = uIndexCount; }
	size_t GetIndexCount() const { return m_uIndexCount; }

	void Allocate();

	DWORD GetIndex(size_t uIndex) const
	{
		if (m_dwIndexSize == 2)
		{
			auto pIndexData16 = reinterpret_cast<const WORD*>(m_pIndexData.get());
			return pIndexData16[uIndex];
		}
		else
		{
			auto pIndexData32 = reinterpret_cast<const DWORD*>(m_pIndexData.get());
			return pIndexData32[uIndex];
		}
	}
	void SetIndex(size_t uIndex, DWORD dwData)
	{
		if (m_dwIndexSize == 2)
		{
			auto pIndexData16 = reinterpret_cast<WORD*>(m_pIndexData.get());
			pIndexData16[uIndex] = static_cast<WORD>(dwData);
		}
		else
		{
			auto pIndexData32 = reinterpret_cast<DWORD*>(m_pIndexData.get());
			pIndexData32[uIndex] = dwData;
		}
	}
	uint8_t* GetIndexData() { return m_pIndexData.get(); }
	const uint8_t* GetIndexData() const { return m_pIndexData.get(); }
	size_t GetIndexDataSize() const { return m_uIndexCount * m_dwIndexSize; }

	void ByteSwap();

protected:
	DWORD                       m_dwIndexSize;
	size_t                      m_uIndexCount;
	std::unique_ptr<uint8_t[]>  m_pIndexData;
};

class TExpMesh
{
public:
	enum OptimizationFlags
	{
		COMPRESS_VERTEX_DATA = 1,
		FLIP_TRIANGLES = 2,
		FORCE_SUBD_CONVERSION = 4,
		CLEAN_MESHES = 8,
		VCACHE_OPT = 16,
	};
	bool                        m_x2Bias;
	std::vector< T_VERTEX_ELEMENT >            m_VertexElements;
	std::vector< D3D11_INPUT_ELEMENT_DESC  >    m_InputLayout;
	std::unique_ptr<ExportVB>                   m_pVB;
	std::unique_ptr<ExportIB>                   m_pIB;
	std::unique_ptr<TVector3[]>        m_pVBPositions;
	std::unique_ptr<TVector3[]>        m_pVBNormals;
	std::unique_ptr<TVector2[]>        m_pVBTexCoords;
public:
	// 2018
	std::vector< T_STR >     m_InfluenceNames;
	std::unordered_map<T_STR, TMatrix> m_matBindPoseMap;
	TVertexFormat        m_VertexFormat;
	void AddInfluence(T_STR InfluenceName, TMatrix& mat)
	{
		m_InfluenceNames.push_back(InfluenceName);
		m_matBindPoseMap[InfluenceName] = mat;
		m_VertexFormat.m_bSkinData = true;
	}
	void SetVertexUVCount(UINT uCount) { m_VertexFormat.m_uUVSetCount = uCount; }
	void SetVertexUVDimension(UINT uDimension) { m_VertexFormat.m_uUVSetSize = uDimension; }
	void SetVertexNormalCount(UINT uCount);
	void SetVertexColorCount(UINT uCount) { m_VertexFormat.m_bVertexColor = (uCount > 0); }

	size_t GetVertexDeclElementCount() const { return m_VertexElements.size(); }
	const T_VERTEX_ELEMENT& GetVertexDeclElement(size_t uIndex) const
	{
		return m_VertexElements[uIndex];
	}
	void BuildVertexBuffer(ExportMeshVertexArray& VertexArray, DWORD dwFlags);
	void TransformPosition(TVector3* pDestPosition, const TVector3* pSrcPosition) const;
	void TransformAndWriteVector(BYTE* pDest, TVector3* normal, const TVector3& Src, DWORD dwDestFormat);
	void WriteColor(BYTE* pDest, const TVector4& Src, DWORD dwDestFormat);
	void TransformDirection(TVector3* pDestDirection, const TVector3* pSrcDirection) const;
	void TransformMatrix(TMatrix* pDestMatrix, const TMatrix* pSrcMatrix) const;
	float TransformLength(float fInputLength) const;
public:
	// Sets unit scale for exporting all geometry - works with characters too.
	void SetUnitScale(const float fScale)
	{
		m_fUnitScale = fScale;
	}

	void SetZFlip(const bool bFlip)
	{
		m_bFlipZ = bFlip;
	}

protected:
	float m_fUnitScale;
	bool  m_bMaxConversion;
	bool  m_bFlipZ;
public:
	TExpMesh();
	virtual ~TExpMesh();
};

