#include "TExpMesh.h"


void NormalizeBoneWeights(BYTE* pWeights)
{
	DWORD dwSum = static_cast<DWORD>(pWeights[0]) + static_cast<DWORD>(pWeights[1]) + static_cast<DWORD>(pWeights[2]) + static_cast<DWORD>(pWeights[3]);
	if (dwSum == 255)
		return;

	INT iDifference = 255 - static_cast<INT>(dwSum);
	for (DWORD i = 0; i < 4; ++i)
	{
		if (pWeights[i] == 0)
			continue;
		INT iValue = static_cast<INT>(pWeights[i]);
		if (iValue + iDifference > 255)
		{
			iDifference -= (255 - iValue);
			iValue = 255;
		}
		else
		{
			iValue += iDifference;
			iDifference = 0;
		}
		pWeights[i] = static_cast<BYTE>(iValue);
	}

	dwSum = static_cast<DWORD>(pWeights[0]) + static_cast<DWORD>(pWeights[1]) + static_cast<DWORD>(pWeights[2]) + static_cast<DWORD>(pWeights[3]);
	assert(dwSum == 255);
}


inline INT GetElementSizeFromDeclType(DWORD Type)
{
	switch (Type)
	{
	case T_TYPE_FLOAT1:
	case T_TYPE_D3DCOLOR:
	case T_TYPE_UBYTE4:
	case T_TYPE_UBYTE4N:
	case T_TYPE_FLOAT16_2:
	/*case T_TYPE_DXGI_R10G10B10A2_UNORM:
	case T_TYPE_DXGI_R11G11B10_FLOAT:
	case T_TYPE_DXGI_R8G8B8A8_SNORM:
	case T_TYPE_XBOX_R10G10B10_SNORM_A2_UNORM:*/
		return 4;

	case T_TYPE_FLOAT2:
	case T_TYPE_SHORT4N:
	case T_TYPE_FLOAT16_4:
		return 8;

	case T_TYPE_FLOAT3:
		return 12;

	case T_TYPE_FLOAT4:
		return 16;

	case T_TYPE_UNUSED:
		return 0;

	default:
		assert(false);
		return 0;
	}
}
void ExportVB::Allocate()
{
	size_t uSize = GetVertexDataSize();
	m_pVertexData.reset(new uint8_t[uSize]);
	ZeroMemory(m_pVertexData.get(), uSize);
}
uint8_t* ExportVB::GetVertex(size_t uIndex)
{
	if (!m_pVertexData)
		return nullptr;
	if (uIndex >= m_uVertexCount)
		return nullptr;
	return m_pVertexData.get() + (uIndex * m_uVertexSizeBytes);
}

const uint8_t* ExportVB::GetVertex(size_t uIndex) const
{
	if (!m_pVertexData)
		return nullptr;
	if (uIndex >= m_uVertexCount)
		return nullptr;
	return m_pVertexData.get() + (uIndex * m_uVertexSizeBytes);
}

void ExportVB::ByteSwap(const T_VERTEX_ELEMENT* pVertexElements, const size_t dwVertexElementCount)
{
	for (size_t dwVertexIndex = 0; dwVertexIndex < m_uVertexCount; dwVertexIndex++)
	{
		auto pVB = GetVertex(dwVertexIndex);
		for (size_t i = 0; i < dwVertexElementCount; i++)
		{
			auto pElement = reinterpret_cast<DWORD*>(pVB + pVertexElements[i].Offset);
			switch (pVertexElements[i].Type)
			{
			case T_TYPE_FLOAT4:
				*pElement = _byteswap_ulong(*pElement);
				pElement++;
			case T_TYPE_FLOAT3:
				*pElement = _byteswap_ulong(*pElement);
				pElement++;
			case T_TYPE_FLOAT2:
				*pElement = _byteswap_ulong(*pElement);
				pElement++;
			case T_TYPE_FLOAT1:
			case T_TYPE_D3DCOLOR:
			case T_TYPE_UBYTE4:
			case T_TYPE_UBYTE4N:
			/*case T_TYPE_DXGI_R10G10B10A2_UNORM:
			case T_TYPE_DXGI_R11G11B10_FLOAT:
			case T_TYPE_DXGI_R8G8B8A8_SNORM:
			case T_TYPE_XBOX_R10G10B10_SNORM_A2_UNORM:*/
				*pElement = _byteswap_ulong(*pElement);
				break;
			case T_TYPE_SHORT4N:
			case T_TYPE_FLOAT16_4:
			{
				auto pWord = reinterpret_cast<WORD*>(pElement);
				*pWord = _byteswap_ushort(*pWord);
				pWord++;
				*pWord = _byteswap_ushort(*pWord);
				pElement++;
			}
			case T_TYPE_FLOAT16_2:
			{
				auto pWord = reinterpret_cast<WORD*>(pElement);
				*pWord = _byteswap_ushort(*pWord);
				pWord++;
				*pWord = _byteswap_ushort(*pWord);
				pElement++;
				break;
			}
			}
		}
	}
}

void ExportIB::ByteSwap()
{
	if (m_dwIndexSize == 2)
	{
		auto pIndexData16 = reinterpret_cast<WORD*>(m_pIndexData.get());
		for (size_t i = 0; i < m_uIndexCount; i++)
		{
			WORD wIndex = _byteswap_ushort(pIndexData16[i]);
			pIndexData16[i] = wIndex;
		}
	}
	else
	{
		auto pIndexData32 = reinterpret_cast<DWORD*>(m_pIndexData.get());
		for (size_t i = 0; i < m_uIndexCount; i++)
		{
			DWORD dwIndex = _byteswap_ulong(pIndexData32[i]);
			pIndexData32[i] = dwIndex;
		}
	}
}

void ExportIB::Allocate()
{
	if (m_dwIndexSize == 2)
	{
		m_pIndexData.reset(reinterpret_cast<uint8_t*>(new WORD[m_uIndexCount]));
		ZeroMemory(m_pIndexData.get(), m_uIndexCount * sizeof(WORD));
	}
	else
	{
		m_pIndexData.reset(reinterpret_cast<uint8_t*>(new DWORD[m_uIndexCount]));
		ZeroMemory(m_pIndexData.get(), m_uIndexCount * sizeof(DWORD));
	}
}
bool ExportMeshVertex::Equals(const ExportMeshVertex* pOtherVertex) const
{
	if (!pOtherVertex)
		return false;

	if (pOtherVertex == this)
		return true;

	TVector3 v0 = Position;
	TVector3 v1 = pOtherVertex->Position;
	if (v0 != v1)
		return false;

	v0 = Normal;
	v1 = pOtherVertex->Normal;
	if (v0 != v1)
		return false;

	TVector4 v2, v3;

	for (size_t i = 0; i < 8; i++)
	{
		v2 = TexCoords[i];
		v3 = pOtherVertex->TexCoords[i];
		if (v2 != v3)
			return false;
	}

	v2 = Color;
	v3 = pOtherVertex->Color;
	if (v2.x != v1.x || v2.y != v1.y || v2.z != v1.z )
		return false;

	return true;
}

void TExpMesh::BuildVertexBuffer(ExportMeshVertexArray& VertexArray, DWORD dwFlags)
{
	UINT uVertexSize = 0;
	INT iCurrentVertexOffset = 0;
	INT iPositionOffset = -1;
	INT iNormalOffset = -1;
	INT iTangentOffset = -1;
	INT iBinormalOffset = -1;
	INT iSkinDataOffset = -1;
	INT iColorOffset = -1;
	INT iUVOffset = -1;

	// create a vertex element struct and set default values
	T_VERTEX_ELEMENT VertexElement;
	ZeroMemory(&VertexElement, sizeof(T_VERTEX_ELEMENT));

	D3D11_INPUT_ELEMENT_DESC InputElement;
	ZeroMemory(&InputElement, sizeof(D3D11_INPUT_ELEMENT_DESC));

	bool bCompressVertexData = (dwFlags & COMPRESS_VERTEX_DATA);

	DWORD dwColorType = T_TYPE_FLOAT4;
	DXGI_FORMAT dwColorTypeDXGI = DXGI_FORMAT_B8G8R8A8_UNORM;

	switch (dwColorType)
	{
	case T_TYPE_FLOAT4:                    dwColorTypeDXGI = DXGI_FORMAT_R32G32B32A32_FLOAT;   break;
	case T_TYPE_D3DCOLOR:                  break;
	case T_TYPE_UBYTE4N:                   dwColorTypeDXGI = DXGI_FORMAT_R8G8B8A8_UNORM;       break;
	case T_TYPE_FLOAT16_4:                 dwColorTypeDXGI = DXGI_FORMAT_R16G16B16A16_FLOAT;   break;
	/*case D3DDECLTYPE_DXGI_R10G10B10A2_UNORM:    dwColorTypeDXGI = DXGI_FORMAT_R10G10B10A2_UNORM;    break;
	case D3DDECLTYPE_DXGI_R11G11B10_FLOAT:      dwColorTypeDXGI = DXGI_FORMAT_R11G11B10_FLOAT;      break;*/
	default:                                    assert(false);                                      break;
	}

	DWORD dwNormalType = T_TYPE_FLOAT3;
	DXGI_FORMAT dwNormalTypeDXGI = DXGI_FORMAT_R32G32B32_FLOAT;
	m_x2Bias = false;
	if (bCompressVertexData)
	{
		dwNormalType = DXGI_FORMAT_R8G8B8A8_UNORM;

		switch (dwNormalType)
		{
		case T_TYPE_UBYTE4N:                       dwNormalTypeDXGI = DXGI_FORMAT_R8G8B8A8_UNORM;      m_x2Bias = true;  break;
		case T_TYPE_SHORT4N:                       dwNormalTypeDXGI = DXGI_FORMAT_R16G16B16A16_SNORM;  break;
		case T_TYPE_FLOAT16_4:                     dwNormalTypeDXGI = DXGI_FORMAT_R16G16B16A16_FLOAT;  break;
		/*case T_TYPE_DXGI_R10G10B10A2_UNORM:        dwNormalTypeDXGI = DXGI_FORMAT_R10G10B10A2_UNORM;   m_x2Bias = true; break;
		case T_TYPE_DXGI_R11G11B10_FLOAT:          dwNormalTypeDXGI = DXGI_FORMAT_R11G11B10_FLOAT;     m_x2Bias = true; break;
		case T_TYPE_DXGI_R8G8B8A8_SNORM:           dwNormalTypeDXGI = DXGI_FORMAT_R8G8B8A8_SNORM;      break;
		case T_TYPE_XBOX_R10G10B10_SNORM_A2_UNORM: dwNormalTypeDXGI = DXGI_FORMAT(189);                break;*/
		default:                                        assert(false);                                      break;
		}
	}

	m_VertexElements.clear();
	m_InputLayout.clear();

	// check each vertex format option, and create a corresponding decl element
	if (m_VertexFormat.m_bPosition)
	{
		iPositionOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_POSITION;
		VertexElement.Type = T_TYPE_FLOAT3;
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "SV_Position";
		InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}
	if (m_VertexFormat.m_bSkinData)
	{
		iSkinDataOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_BLENDWEIGHT;
		VertexElement.Type = T_TYPE_UBYTE4N;
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "BLENDWEIGHT";
		InputElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_BLENDINDICES;
		VertexElement.Type = T_TYPE_UBYTE4;
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "BLENDINDICES";
		InputElement.Format = DXGI_FORMAT_R8G8B8A8_UINT;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}

	if (m_VertexFormat.m_bNormal)
	{
		iNormalOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_NORMAL;
		VertexElement.Type = static_cast<BYTE>(dwNormalType);
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "NORMAL";
		InputElement.Format = dwNormalTypeDXGI;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}
	if (m_VertexFormat.m_bVertexColor)
	{
		iColorOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_COLOR;
		VertexElement.Type = static_cast<BYTE>(dwColorType);
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "COLOR";
		InputElement.Format = dwColorTypeDXGI;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}
	if (m_VertexFormat.m_uUVSetCount > 0)
	{
		iUVOffset = iCurrentVertexOffset;
		for (UINT t = 0; t < m_VertexFormat.m_uUVSetCount; t++)
		{
			VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
			VertexElement.Usage = T_USAGE_TEXCOORD;

			InputElement.SemanticName = "TEXCOORD";
			InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);

			switch (m_VertexFormat.m_uUVSetSize)
			{
			case 1:
				VertexElement.Type = T_TYPE_FLOAT1;
				InputElement.Format = DXGI_FORMAT_R32_FLOAT;
				break;

			case 2:
				if (bCompressVertexData)
				{
					VertexElement.Type = T_TYPE_FLOAT16_2;
					InputElement.Format = DXGI_FORMAT_R16G16_FLOAT;
				}
				else
				{
					VertexElement.Type = T_TYPE_FLOAT2;
					InputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
				}
				break;

			case 3:
				if (bCompressVertexData)
				{
					VertexElement.Type = T_TYPE_FLOAT16_4;
					InputElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				else
				{
					VertexElement.Type = T_TYPE_FLOAT3;
					InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				break;

			case 4:
				if (bCompressVertexData)
				{
					VertexElement.Type = T_TYPE_FLOAT16_4;
					InputElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				else
				{
					VertexElement.Type = T_TYPE_FLOAT4;
					InputElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				break;

			default:
				continue;
			}

			VertexElement.UsageIndex = static_cast<BYTE>(t);
			m_VertexElements.push_back(VertexElement);

			InputElement.SemanticIndex = t;
			m_InputLayout.push_back(InputElement);

			iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
		}
		VertexElement.UsageIndex = 0;
		InputElement.SemanticIndex = 0;
	}
	if (m_VertexFormat.m_bTangent)
	{
		iTangentOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_TANGENT;
		VertexElement.Type = static_cast<BYTE>(dwNormalType);
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "TANGENT";
		InputElement.Format = dwNormalTypeDXGI;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}
	if (m_VertexFormat.m_bBinormal)
	{
		iBinormalOffset = iCurrentVertexOffset;

		VertexElement.Offset = static_cast<WORD>(iCurrentVertexOffset);
		VertexElement.Usage = T_USAGE_BINORMAL;
		VertexElement.Type = static_cast<BYTE>(dwNormalType);
		m_VertexElements.push_back(VertexElement);

		InputElement.SemanticName = "BINORMAL";
		InputElement.Format = dwNormalTypeDXGI;
		InputElement.AlignedByteOffset = static_cast<UINT>(iCurrentVertexOffset);
		m_InputLayout.push_back(InputElement);

		iCurrentVertexOffset += GetElementSizeFromDeclType(VertexElement.Type);
	}

	assert(m_VertexElements.size() == m_InputLayout.size());

	// save vertex size
	uVertexSize = iCurrentVertexOffset;
	if (uVertexSize == 0)
		return;

	// create vertex buffer and allocate storage
	size_t nVerts = VertexArray.size();

	m_pVB = std::make_unique<ExportVB>();
	m_pVB->SetVertexCount(nVerts);
	m_pVB->SetVertexSize(uVertexSize);
	m_pVB->Allocate();

	if (iPositionOffset != -1)
	{
		m_pVBPositions.reset(new TVector3[nVerts]);
		memset(m_pVBPositions.get(), 0, sizeof(TVector3) * nVerts);
	}

	if (iNormalOffset != -1)
	{
		m_pVBNormals.reset(new TVector3[nVerts]);
		memset(m_pVBNormals.get(), 0, sizeof(TVector3) * nVerts);
	}

	if (iUVOffset != -1)
	{
		m_pVBTexCoords.reset(new TVector2[nVerts]);
		memset(m_pVBTexCoords.get(), 0, sizeof(TVector2) * nVerts);
	}

	// copy raw vertex data into the packed vertex buffer
	for (size_t i = 0; i < nVerts; i++)
	{
		auto pDestVertex = m_pVB->GetVertex(i);
		ExportMeshVertex* pSrcVertex = VertexArray[i];
		if (!pSrcVertex)
		{
			continue;
		}

		if (iPositionOffset != -1)
		{
			auto pDest = reinterpret_cast<TVector3*>(pDestVertex + iPositionOffset);
			TransformPosition(pDest, &pSrcVertex->Position);

			memcpy(&m_pVBPositions[i], pDest, sizeof(TVector2));
		}
		if (iNormalOffset != -1)
		{
			TransformAndWriteVector(pDestVertex + iNormalOffset, &m_pVBNormals[i], pSrcVertex->Normal, dwNormalType);
		}
		if (iSkinDataOffset != -1)
		{
			BYTE* pDest = pDestVertex + iSkinDataOffset;
			BYTE* pBoneWeights = pDest;
			*pDest++ = static_cast<BYTE>(pSrcVertex->BoneWeights.x * 255.0f);
			*pDest++ = static_cast<BYTE>(pSrcVertex->BoneWeights.y * 255.0f);
			*pDest++ = static_cast<BYTE>(pSrcVertex->BoneWeights.z * 255.0f);
			*pDest++ = static_cast<BYTE>(pSrcVertex->BoneWeights.w * 255.0f);
			NormalizeBoneWeights(pBoneWeights);
			*pDest++ = (BYTE)pSrcVertex->BoneIndices.x;
			*pDest++ = (BYTE)pSrcVertex->BoneIndices.y;
			*pDest++ = (BYTE)pSrcVertex->BoneIndices.z;
			*pDest++ = (BYTE)pSrcVertex->BoneIndices.w;
		}
		if (iTangentOffset != -1)
		{
			TransformAndWriteVector(pDestVertex + iTangentOffset, nullptr, pSrcVertex->Tangent, dwNormalType);
		}
		if (iBinormalOffset != -1)
		{
			TransformAndWriteVector(pDestVertex + iBinormalOffset, nullptr, pSrcVertex->Binormal, dwNormalType);
		}
		if (iColorOffset != -1)
		{
			WriteColor(pDestVertex + iColorOffset, pSrcVertex->Color, dwColorType);
		}
		if (iUVOffset != -1)
		{
			UINT iTangentSpaceIndex = 0;
			if (m_VertexFormat.m_uUVSetCount > iTangentSpaceIndex)
			{
				if (m_VertexFormat.m_uUVSetSize > 1)
				{
					memcpy(&m_pVBTexCoords[i], &pSrcVertex->TexCoords[iTangentSpaceIndex], sizeof(TVector2));
				}
			}

			/*if (bCompressVertexData)
			{
				auto pDest = reinterpret_cast<DWORD*>(pDestVertex + iUVOffset);
				for (size_t t = 0; t < m_VertexFormat.m_uUVSetCount; t++)
				{
					switch (m_VertexFormat.m_uUVSetSize)
					{
					case 1:
					{
						memcpy(pDest, &pSrcVertex->TexCoords[t], sizeof(float));
						pDest++;
						break;
					}
					case 2:
					{
						auto pFloat16 = reinterpret_cast<HALF*>(pDest);
						XMConvertFloatToHalfStream(pFloat16, sizeof(HALF), reinterpret_cast<const float*>(&pSrcVertex->TexCoords[t]), sizeof(float), 2);
						pDest++;
						break;
					}
					case 3:
					{
						pDest[1] = 0;
						auto pFloat16 = reinterpret_cast<HALF*>(pDest);
						XMConvertFloatToHalfStream(pFloat16, sizeof(HALF), reinterpret_cast<const float*>(&pSrcVertex->TexCoords[t]), sizeof(float), 3);
						pDest += 2;
						break;
					}
					case 4:
					{
						auto pFloat16 = reinterpret_cast<HALF*>(pDest);
						XMConvertFloatToHalfStream(pFloat16, sizeof(HALF), reinterpret_cast<const float*>(&pSrcVertex->TexCoords[t]), sizeof(float), 4);
						pDest += 2;
						break;
					}
					default:
						assert(false);
						break;
					}
				}
			}
			else*/
			{
				size_t uStride = m_VertexFormat.m_uUVSetSize * sizeof(float);
				BYTE* pDest = pDestVertex + iUVOffset;
				for (UINT t = 0; t < m_VertexFormat.m_uUVSetCount; t++)
				{
					memcpy(pDest, &pSrcVertex->TexCoords[t], uStride);
					pDest += uStride;
				}
			}
		}
	}
}
void TExpMesh::WriteColor(BYTE* pDest, const TVector4& Src, DWORD dwDestFormat)
{
	/*static const XMVECTORF32 s_8BitBias = { 0.5f / 255.f, 0.5f / 255.f, 0.5f / 255.f, 0.5f / 255.f };

	switch (dwDestFormat)
	{
	case D3DDECLTYPE_D3DCOLOR:
	{
		XMVECTOR clr = XMLoadFloat4(&Src);
		clr = XMVectorAdd(clr, s_8BitBias);
		XMStoreColor(reinterpret_cast<XMCOLOR*>(pDest), clr);
		break;
	}
	case D3DDECLTYPE_UBYTE4N:
	{
		XMVECTOR clr = XMLoadFloat4(&Src);
		clr = XMVectorAdd(clr, s_8BitBias);
		XMStoreUByteN4(reinterpret_cast<XMUBYTEN4*>(pDest), clr);
		break;
	}
	case D3DDECLTYPE_FLOAT4:
		*reinterpret_cast<TVector4*>(pDest) = Src;
		break;
	case D3DDECLTYPE_FLOAT16_4:
		*reinterpret_cast<TVector4*>(pDest) = TVector4(Src.x, Src.y, Src.z, Src.z);
		break;
	case D3DDECLTYPE_DXGI_R10G10B10A2_UNORM:
	{
		XMVECTOR clr = XMLoadFloat4(&Src);
		XMStoreUDecN4(reinterpret_cast<XMUDECN4*>(pDest), clr);
		break;
	}
	case D3DDECLTYPE_DXGI_R11G11B10_FLOAT:
	{
		XMVECTOR clr = XMLoadFloat4(&Src);
		XMStoreFloat3PK(reinterpret_cast<XMFLOAT3PK*>(pDest), clr);
		break;
	}
	default:
		assert(false);
		break;
	}*/
}
void TExpMesh::TransformAndWriteVector(BYTE* pDest, TVector3* normal, const TVector3& Src, DWORD dwDestFormat)
{
	TVector3 SrcTransformed;
	TransformDirection(&SrcTransformed, &Src);

	if (normal)
	{
		memcpy(normal, &SrcTransformed, sizeof(TVector3));
	}

	switch (dwDestFormat)
	{
	case T_TYPE_FLOAT3:
	{
		*reinterpret_cast<TVector3*>(pDest) = SrcTransformed;
		break;
	}
	//case T_TYPE_UBYTE4N: // Biased to get normals [-1,1] into range
	//{
	//	TVector3 v = SrcTransformed;
	//	v = v * g_XMOneHalf;
	//	v += g_XMOneHalf;
	//	v = XMVectorSelect(g_XMOne, v, g_XMSelect1110);
	//	XMStoreUByteN4(reinterpret_cast<XMUBYTEN4*>(pDest), v);
	//	break;
	//}
	//case T_TYPE_SHORT4N:
	//{
	//	*reinterpret_cast<XMSHORTN4*>(pDest) = XMSHORTN4(SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1);
	//	break;
	//}
	//case T_TYPE_FLOAT16_4:
	//{
	//	*reinterpret_cast<XMHALF4*>(pDest) = XMHALF4(SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1);
	//	break;
	//}
	//case T_TYPE_DXGI_R10G10B10A2_UNORM: // Biased to get normals [-1,1] into range
	//{
	//	XMVECTOR v = XMLoadFloat3(&SrcTransformed);
	//	v = v * g_XMOneHalf;
	//	v += g_XMOneHalf;

	//	XMStoreUDecN4(reinterpret_cast<XMUDECN4*>(pDest), v);
	//	break;
	//}
	//case T_TYPE_DXGI_R11G11B10_FLOAT: // Biased to get normals [-1,1] into range
	//{
	//	XMVECTOR v = XMLoadFloat3(&SrcTransformed);
	//	v = v * g_XMOneHalf;
	//	v += g_XMOneHalf;

	//	XMStoreFloat3PK(reinterpret_cast<XMFLOAT3PK*>(pDest), v);
	//	break;
	//}
	//case T_TYPE_DXGI_R8G8B8A8_SNORM:
	//{
	//	*reinterpret_cast<XMBYTEN4*>(pDest) = XMBYTEN4(SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1);
	//	break;
	//}
	//case D3DDECLTYPE_XBOX_R10G10B10_SNORM_A2_UNORM:
	//{
	//	// Xbox One specific format
	//	*reinterpret_cast<XMXDECN4*>(pDest) = XMXDECN4(SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1);
	//	break;
	//}
	default:
		assert(false);
		break;
	}
}
void TExpMesh::TransformPosition(TVector3* pDestPosition, const TVector3* pSrcPosition) const
{
	TVector3 SrcVector;
	if (pSrcPosition == pDestPosition)
	{
		SrcVector = *pSrcPosition;
		pSrcPosition = &SrcVector;
	}

	if (m_bMaxConversion)
	{
		pDestPosition->x = pSrcPosition->x * m_fUnitScale;
		pDestPosition->y = pSrcPosition->z * m_fUnitScale;
		pDestPosition->z = pSrcPosition->y * m_fUnitScale;
	}
	else
	{
		const float flipZ = m_bFlipZ ? -1.0f : 1.0f;

		pDestPosition->x = pSrcPosition->x * m_fUnitScale;
		pDestPosition->y = pSrcPosition->y * m_fUnitScale;
		pDestPosition->z = pSrcPosition->z * m_fUnitScale * flipZ;
	}
}

void TExpMesh::TransformDirection(TVector3* pDestDirection, const TVector3* pSrcDirection) const
{
	TVector3 SrcVector;
	if (pSrcDirection == pDestDirection)
	{
		SrcVector = *pSrcDirection;
		pSrcDirection = &SrcVector;
	}

	if (m_bMaxConversion)
	{
		pDestDirection->x = pSrcDirection->x;
		pDestDirection->y = pSrcDirection->z;
		pDestDirection->z = pSrcDirection->y;
	}
	else
	{
		const float flipZ = m_bFlipZ ? -1.0f : 1.0f;

		pDestDirection->x = pSrcDirection->x;
		pDestDirection->y = pSrcDirection->y;
		pDestDirection->z = pSrcDirection->z * flipZ;
	}
}
void TExpMesh::TransformMatrix(TMatrix* pDestMatrix, const TMatrix* pSrcMatrix) const
{
	TMatrix SrcMatrix;
	if (pSrcMatrix == pDestMatrix)
	{
		memcpy(&SrcMatrix, pSrcMatrix, sizeof(TMatrix));
		pSrcMatrix = &SrcMatrix;
	}
	memcpy(pDestMatrix, pSrcMatrix, sizeof(TMatrix));

	// What we're doing here is premultiplying by a left hand -> right hand matrix,
	// and then postmultiplying by a right hand -> left hand matrix.
	// The end result of those multiplications is that the third row and the third
	// column are negated (so element _33 is left alone).  So instead of actually
	// carrying out the multiplication, we just negate the 6 matrix elements.

	if (m_bFlipZ)
	{
		pDestMatrix->_13 = -pSrcMatrix->_13;
		pDestMatrix->_23 = -pSrcMatrix->_23;
		pDestMatrix->_43 = -pSrcMatrix->_43;

		pDestMatrix->_31 = -pSrcMatrix->_31;
		pDestMatrix->_32 = -pSrcMatrix->_32;
		pDestMatrix->_34 = -pSrcMatrix->_34;
	}

	// Apply the global unit scale to the translation components of the matrix.
	pDestMatrix->_41 *= m_fUnitScale;
	pDestMatrix->_42 *= m_fUnitScale;
	pDestMatrix->_43 *= m_fUnitScale;
}
float TExpMesh::TransformLength(float fInputLength) const
{
	return fInputLength * m_fUnitScale;
}
TExpMesh::TExpMesh() : m_fUnitScale(1.0f), m_bFlipZ(true), m_bMaxConversion(false)
{
}


TExpMesh::~TExpMesh()
{
	m_InfluenceNames.clear();
}
