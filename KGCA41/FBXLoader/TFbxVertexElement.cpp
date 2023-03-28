#include "TFbxObj.h"
BOOL TFbxObj::ExtractTextures(FbxProperty Property, const CHAR* strParameterName, TFbxMaterial* pMaterial, DWORD dwFlags)
{
	BOOL bResult = FALSE;
	const DWORD dwLayeredTextureCount = Property.GetSrcObjectCount<FbxLayeredTexture>();
	if (dwLayeredTextureCount > 0)
	{
		DWORD dwTextureIndex = 0;
		for (DWORD i = 0; i < dwLayeredTextureCount; ++i)
		{
			FbxLayeredTexture* pFbxLayeredTexture = FbxCast<FbxLayeredTexture>(Property.GetSrcObject<FbxLayeredTexture>(i));
			DWORD dwTextureCount = pFbxLayeredTexture->GetSrcObjectCount<FbxTexture>();
			for (DWORD j = 0; j < dwTextureCount; ++j)
			{
				FbxTexture* pFbxTexture = FbxCast<FbxTexture>(pFbxLayeredTexture->GetSrcObject<FbxTexture>(j));
				if (pFbxTexture == NULL)
					continue;
				const FbxFileTexture* lTexture = Property.GetSrcObject<FbxFileTexture>(j);
				AddTextureParameter(pMaterial, strParameterName, dwTextureIndex, lTexture->GetFileName(), dwFlags);
				++dwTextureIndex;
				bResult = TRUE;
			}
		}
	}
	else
	{
		DWORD dwTextureCount = Property.GetSrcObjectCount<FbxTexture>();
		for (DWORD i = 0; i < dwTextureCount; ++i)
		{
			FbxTexture* pFbxTexture = FbxCast<FbxTexture>(Property.GetSrcObject<FbxTexture>(i));
			if (pFbxTexture == NULL)
				continue;
			const FbxFileTexture* lTexture = Property.GetSrcObject<FbxFileTexture>(i);
			AddTextureParameter(pMaterial, strParameterName, i, lTexture->GetFileName(), dwFlags);
			bResult = TRUE;
		}
	}
	return bResult;
}
VOID TFbxObj::AddTextureParameter(TFbxMaterial* pMaterial, const CHAR* strParamName, DWORD dwIndex, const CHAR* strFileName, DWORD dwFlags)
{
	ExportMaterialParameter OutputParam;
	if (dwIndex == 0)
	{
		OutputParam.Name = strParamName;
	}
	else
	{
		CHAR strDecoratedName[512];
		sprintf_s(strDecoratedName, "%s%d", strParamName, dwIndex);
		OutputParam.Name = strDecoratedName;
	}
	OutputParam.ValueString = strFileName;
	OutputParam.ParamType = MPT_TEXTURE2D;
	OutputParam.bInstanceParam = TRUE;
	OutputParam.Flags = dwFlags;
	pMaterial->AddParameter(OutputParam);
}
TFbxMaterial* TFbxObj::ParseMaterialInLayer(FbxMesh* pMesh, FbxLayer* pLayer, DWORD dwMaterialIndex)
{
	FbxLayerElementMaterial* pLayerMaterials = pLayer->GetMaterials();
	//FbxSurfaceMaterial* pFbxMateria1 = (FbxSurfaceMaterial*)pLayerMaterials.GetDirectArray();
	FbxSurfaceMaterial* pFbxMaterial = pMesh->GetNode()->GetMaterial((INT)dwMaterialIndex);
	assert(pFbxMaterial != NULL);
	BOOL bRenameMaterial = FALSE;
	string MaterialName(pFbxMaterial->GetName());
	TFbxMaterial* pSameNameMaterial = NULL;
	DWORD dwRenameIndex = 0;

	bRenameMaterial = TRUE;
	CHAR strName[200];
	sprintf_s(strName, "%s_%d", pFbxMaterial->GetName(), dwRenameIndex++);
	MaterialName = strName;

	TFbxMaterial* pMaterial = new TFbxMaterial(MaterialName);

	enum ParameterPostOperations
	{
		PPO_Nothing = 0,
		PPO_TransparentMaterial = 1,
	};

	struct TextureParameterExtraction
	{
		const CHAR* strFbxPropertyName;
		const CHAR* strParameterName;
		DWORD dwPostOperations;
		DWORD dwParameterFlags;
	};
	TextureParameterExtraction ExtractionList[] =
	{
		{ FbxSurfaceMaterial::sTransparentColor,   "AlphaTexture",                 PPO_TransparentMaterial,    ExportMaterialParameter::EMPF_ALPHACHANNEL },
		{ FbxSurfaceMaterial::sDiffuse,            "DiffuseTexture",               PPO_Nothing,                ExportMaterialParameter::EMPF_DIFFUSEMAP },
		{ FbxSurfaceMaterial::sBump,               "NormalMapTexture",             PPO_Nothing,                0 /*ExportMaterialParameter::EMPF_BUMPMAP*/ },
		{ FbxSurfaceMaterial::sNormalMap,          "NormalMapTexture",             PPO_Nothing,                ExportMaterialParameter::EMPF_NORMALMAP },
		{ FbxSurfaceMaterial::sSpecular,           "SpecularMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
		{ FbxSurfaceMaterial::sEmissive,           "EmissiveMapTexture",           PPO_Nothing,                0 },
		{ FbxSurfaceMaterial::sTransparencyFactor,           "MaskTexture",           PPO_TransparentMaterial, 0},               
	};
	for (DWORD dwExtractionIndex = 0; dwExtractionIndex < ARRAYSIZE(ExtractionList); ++dwExtractionIndex)
	{
		const TextureParameterExtraction& tpe = ExtractionList[dwExtractionIndex];
		FbxProperty Property = pFbxMaterial->FindProperty(tpe.strFbxPropertyName);

		if (!Property.IsValid())
			continue;

		BOOL bFound = ExtractTextures(Property, tpe.strParameterName, pMaterial, tpe.dwParameterFlags);
		if (bFound)
		{
			if (tpe.dwPostOperations & PPO_TransparentMaterial)
			{
				pMaterial->SetTransparent(TRUE);
			}
		}
	}
	return pMaterial;
}
/*
GetMappingMode() 요소가 표면에 매핑되는 방식을 결정한다.
eNone			: 매핑이 결정되지 않았다.
eByControlPoint : 각 표면(surface) 제어점/정점에 대해 하나의 매핑 좌표가 있다.
eByPolygonVertex: 각 정점에 대해 하나의 매핑 좌표가 있으며, 그 일부가 되는 모든 다각형(polygon)에 대해 있다. 
                  즉, 정점은 자신이 속한 다각형 만큼 많은 매핑 좌표를 갖게된다.
eByPolygon		: 전체 다각형(polygon)에 대해 하나의 매핑 좌표 만있을 수 있다.
eByEdge			: 메시의 각 고유 가장자리에 대해 하나의 매핑 좌표가 있다. 
				  이것은 스무딩 레이어 요소와 함께 사용하기 위한 것이다.
eAllSame		: 전체 표면(surface)에 대해 하나의 매핑 좌표 만있을 수 있다.
*/
/*
GetReferenceMode() 매핑 정보가 좌표 배열에 저장되는 방식을 결정한다.
eDirect			: n 번째 요소의 매핑 정보가 FbxLayerElementTemplate::mDirectArray의 n 번째 위치에 있음을 나타낸다.
eIndex,			: 이 기호는 FBX v5.0 파일과의 하위 호환성을 위해 유지된다. 
				  FBX v6.0 이상에서는이 기호가 eIndexToDirect로 대체된다.
eIndexToDirect	: 이것은 FbxLayerElementTemplate::mIndexArray에 n 번째 요소에 대해 
				  매핑 요소의 FbxLayerElementTemplate::mDirectArray 배열에 인덱스가 포함되어 있음을 나타낸다.
				  eIndexToDirect는 일반적으로 eByPolygonVertex 매핑 모드 요소 좌표를 저장하는 데 유용하다. 
				  동일한 좌표는 일반적으로 여러 번 반복되기 때문에 좌표를 한 번만 저장하고 인덱스로 참조하여 공간을 절약한다. 
				  머티리얼과 텍스처도이 모드로 참조되며 실제 머티리얼 / 텍스처는 FbxLayerElementTemplate::mDirectArray를 통해 액세스 할 수 있다.
*/
FbxVector2 TFbxObj::ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 uv(0, 0);
	if (dwVertexTextureCount < 1 || pUVSet == nullptr)
	{
		return uv;
	}
	int iVertexTextureCountLayer = pFbxMesh->GetElementUVCount();
	FbxLayerElementUV *pFbxLayerElementUV = pFbxMesh->GetElementUV(0);

	// 제어점은 평면의 4개 정점, 폴리곤 정점은 6개 정점을 위미한다.
	// 그래서 텍스처 좌표와 같은 레이어 들은 제어점 또는 정점으로 구분된다.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // 제어점 당 1개의 텍스처 좌표가 있다.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // 배열에서 직접 얻는다.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // 배열에 해당하는 인덱스를 통하여 얻는다.
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
	case FbxLayerElementUV::eByPolygonVertex: // 정점 당 1개의 매핑 좌표가 있다.
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
	int dwVertexColorCountLayer = mesh->GetElementVertexColorCount();
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
