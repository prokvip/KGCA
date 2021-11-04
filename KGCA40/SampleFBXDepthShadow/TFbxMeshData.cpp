#include "TFbxObj.h"

FbxVector2 TFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 uv(0, 0);
	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
	{
		return uv;
	}
	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
	FbxLayerElementUV* pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

	// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점을 위미한다.
	// 그래서 텍스처 좌표와 같은 레이어 들은 제어점 또는 정점으로 구분된다.
	switch (pUVSet->GetMappingMode())
	{
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pUVSet->GetReferenceMode())
		{
			// 배열에서 직접 얻는다.
		case FbxLayerElementUV::eDirect:
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		// 배열에 해당하는 인덱스를 통하여 얻는다.
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = pUVSet->GetIndexArray().GetAt(vertexIndex);
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(id);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		}
		break;
	}
	// 정점 당 1개의 매핑 좌표가 있다.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv.mData[0] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[0];
			uv.mData[1] = pUVSet->GetDirectArray().GetAt(uvIndex).mData[1];
			break;
		}
		}
		break;
	}
	}
	return uv;
}
// 정점 노말을 읽는 함수 
FbxVector4 TFbxObj::ReadNormal(const FbxMesh* mesh,
	DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
	int controlPointIndex, int iVertexIndex)
{
	FbxVector4 result(0, 0, 0);
	if (dwVertexNormalCount < 1)
	{
		return result;
	}
	int iVertexNormalLayer = mesh->GetElementNormalCount();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	// 노말 획득 

	// 노말 벡터를 저장할 벡터 
	switch (VertexNormalSets->GetMappingMode()) 	// 매핑 모드 
	{
		// 제어점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByControlPoint:
	{
		// control point mapping 
		switch (VertexNormalSets->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		} break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(controlPointIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// 정점 마다 1개의 매핑 좌표가 있다.
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(iVertexIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = VertexNormalSets->GetIndexArray().GetAt(iVertexIndex);
			// 인덱스를 얻어온다. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}
	}break;
	}
	return result;
}
FbxColor TFbxObj::ReadColor(const FbxMesh* mesh,
	DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxColor Value(1, 1, 1, 1);
	FbxVector4 ret(0, 0, 0);
	if (dwVertexColorCount < 1)
	{
		return Value;
	}
	int dwVertexColorCountLayer = mesh->GetElementNormalCount();
	const FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor(0);

	if (dwVertexColorCount > 0 && VertexColorSets != NULL)
	{
		// Crack apart the FBX dereferencing system for Color coordinates		
		switch (VertexColorSets->GetMappingMode())
		{
		case FbxLayerElement::eByControlPoint:
			switch (VertexColorSets->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				Value = VertexColorSets->GetDirectArray().GetAt(dwDCCIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = VertexColorSets->GetIndexArray().GetAt(dwDCCIndex);
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
		case FbxLayerElement::eByPolygonVertex:
			switch (VertexColorSets->GetReferenceMode())
			{
			case FbxLayerElement::eDirect:
			{
				int iColorIndex = dwVertexIndex;
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				int iColorIndex = VertexColorSets->GetIndexArray().GetAt(dwVertexIndex);
				Value = VertexColorSets->GetDirectArray().GetAt(iColorIndex);
			}break;
			}
			break;
		}
	}
	return Value;
}

FbxVector4 TFbxObj::ReadTangent(const FbxMesh* mesh,
	DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
	DWORD dwDCCIndex, DWORD dwVertexIndex)
{
	FbxVector4 ret(0, 0, 0);
	if (dwVertexTangentCount < 1)
	{
		return ret;
	}
	int dwVertexTangentCountLayer = mesh->GetElementTangentCount();
	const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	if (vertexTangent != nullptr)
	{
		switch (vertexTangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				ret[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(dwDCCIndex).mData[0]);
				ret[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(dwDCCIndex).mData[1]);
				ret[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(dwDCCIndex).mData[2]);
			}break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(dwDCCIndex);
				ret[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				ret[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				ret[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}break;
			default:
			{
				assert(0);
			}break;
			}break;
		}

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				int iTangentIndex = dwVertexIndex;
				ret[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[0]);
				ret[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[1]);
				ret[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[2]);
			} break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int iTangentIndex = vertexTangent->GetIndexArray().GetAt(dwVertexIndex);
				ret[0] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[0]);
				ret[1] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[1]);
				ret[2] = static_cast<float>(vertexTangent->GetDirectArray().GetAt(iTangentIndex).mData[2]);
			} break;
			default:
			{
				assert(0);
			}
			} break;
		}
		}
	}
	return ret;
}
