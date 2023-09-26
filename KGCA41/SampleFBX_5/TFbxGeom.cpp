#include "TFbxImport.h"
int TFbxImport::GetSubMaterialIndex(UINT iPoly, FbxLayerElementMaterial* mtrl)
{
	int iSubMtrl = 0;
	switch (mtrl->GetMappingMode())
	{
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByPolygon:
	{
		switch (mtrl->GetReferenceMode())
		{
		case FbxLayerElementUV::eIndex: //배열에서 직접 얻는다.
		{
			iSubMtrl = iPoly;
		}break;
		case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
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
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //배열에서 직접 얻는다.
		{
			normal = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// 정점 당 1개의 텍스처 좌표가 있다.
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
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //배열에서 직접 얻는다.
		{
			color = layer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			color = layer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// 정점 당 1개의 텍스처 좌표가 있다.
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
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //배열에서 직접 얻는다.
		{
			uv = uvLayer->GetDirectArray().GetAt(iVertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
		{
			int id = uvLayer->GetIndexArray().GetAt(iVertexIndex);
			uv = uvLayer->GetDirectArray().GetAt(id);
		}break;
		}
	}break;
	// 정점 당 1개의 텍스처 좌표가 있다.
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
