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
GetMappingMode() ��Ұ� ǥ�鿡 ���εǴ� ����� �����Ѵ�.
eNone			: ������ �������� �ʾҴ�.
eByControlPoint : �� ǥ��(surface) ������/������ ���� �ϳ��� ���� ��ǥ�� �ִ�.
eByPolygonVertex: �� ������ ���� �ϳ��� ���� ��ǥ�� ������, �� �Ϻΰ� �Ǵ� ��� �ٰ���(polygon)�� ���� �ִ�. 
                  ��, ������ �ڽ��� ���� �ٰ��� ��ŭ ���� ���� ��ǥ�� ���Եȴ�.
eByPolygon		: ��ü �ٰ���(polygon)�� ���� �ϳ��� ���� ��ǥ ������ �� �ִ�.
eByEdge			: �޽��� �� ���� �����ڸ��� ���� �ϳ��� ���� ��ǥ�� �ִ�. 
				  �̰��� ������ ���̾� ��ҿ� �Բ� ����ϱ� ���� ���̴�.
eAllSame		: ��ü ǥ��(surface)�� ���� �ϳ��� ���� ��ǥ ������ �� �ִ�.
*/
/*
GetReferenceMode() ���� ������ ��ǥ �迭�� ����Ǵ� ����� �����Ѵ�.
eDirect			: n ��° ����� ���� ������ FbxLayerElementTemplate::mDirectArray�� n ��° ��ġ�� ������ ��Ÿ����.
eIndex,			: �� ��ȣ�� FBX v5.0 ���ϰ��� ���� ȣȯ���� ���� �����ȴ�. 
				  FBX v6.0 �̻󿡼����� ��ȣ�� eIndexToDirect�� ��ü�ȴ�.
eIndexToDirect	: �̰��� FbxLayerElementTemplate::mIndexArray�� n ��° ��ҿ� ���� 
				  ���� ����� FbxLayerElementTemplate::mDirectArray �迭�� �ε����� ���ԵǾ� ������ ��Ÿ����.
				  eIndexToDirect�� �Ϲ������� eByPolygonVertex ���� ��� ��� ��ǥ�� �����ϴ� �� �����ϴ�. 
				  ������ ��ǥ�� �Ϲ������� ���� �� �ݺ��Ǳ� ������ ��ǥ�� �� ���� �����ϰ� �ε����� �����Ͽ� ������ �����Ѵ�. 
				  ��Ƽ����� �ؽ�ó���� ���� �����Ǹ� ���� ��Ƽ���� / �ؽ�ó�� FbxLayerElementTemplate::mDirectArray�� ���� �׼��� �� �� �ִ�.
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

	// �������� ����� 4�� ����, ������ ������ 6�� ������ �����Ѵ�.
	// �׷��� �ؽ�ó ��ǥ�� ���� ���̾� ���� ������ �Ǵ� �������� ���еȴ�.
	switch (pUVSet->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	{
		switch (pUVSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: // �迭���� ���� ��´�.
		{
			FbxVector2 fbxUv = pUVSet->GetDirectArray().GetAt(vertexIndex);
			uv.mData[0] = fbxUv.mData[0];
			uv.mData[1] = fbxUv.mData[1];
			break;
		}
		case FbxLayerElementUV::eIndexToDirect: // �迭�� �ش��ϴ� �ε����� ���Ͽ� ��´�.
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
	case FbxLayerElementUV::eByPolygonVertex: // ���� �� 1���� ���� ��ǥ�� �ִ�.
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
// ���� �븻�� �д� �Լ� 
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
	// �븻 ȹ�� 

	// �븻 ���͸� ������ ���� 
	switch (VertexNormalSets->GetMappingMode()) 	// ���� ��� 
	{
		// ������ ���� 1���� ���� ��ǥ�� �ִ�.
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
			// �ε����� ���´�. 
			result[0] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[0]);
			result[1] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[1]);
			result[2] = static_cast<float>(VertexNormalSets->GetDirectArray().GetAt(index).mData[2]);
		}break;
		}break;
	}break;
	// ���� ���� 1���� ���� ��ǥ�� �ִ�.
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
			// �ε����� ���´�. 
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
