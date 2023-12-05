#include "TFbxImport.h"
int TFbxImport::GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial* mtrl)
{
	int iSubMtrl = 0;
	switch (mtrl->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygon:
	{
		switch (mtrl->GetReferenceMode())
		{
		case FbxLayerElementUV::eIndex: //�迭���� ���� ��´�.
		{
			iSubMtrl = iPoly;
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			iSubMtrl = mtrl->GetIndexArray().GetAt(iPoly);
		}break;
		}
	}break;
	default:
	{
		break;
	}
	}
	return iSubMtrl;
}
C_STR TFbxImport::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	C_STR texName;
	auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			C_STR filename = tex->GetFileName();
			return filename;
		}
	}
	return texName;
}
FbxVector4  TFbxImport::ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex)
{
	FbxVector4 normal;
	switch (layer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{
			normal = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	}
	return normal;
}
FbxColor	TFbxImport::ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex,
	int iIndex)
{
	FbxColor color(1, 1, 1, 1);
	switch (layer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{
			color = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			color = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			color = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	}
	return color;
}
FbxVector2 TFbxImport::ReadTextureCoord(FbxLayerElementUV* uvLayer, int iVertexIndex, int uvIndex)
{
	FbxVector2 uv;
	switch (uvLayer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{
			uv = uvLayer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{
			int id = uvLayer->GetIndexArray().GetAt(iVertexIndex);
			uv = uvLayer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv = uvLayer->GetDirectArray().GetAt(uvIndex);
		}break;
		}
	}break;
	}
	return uv;
}
