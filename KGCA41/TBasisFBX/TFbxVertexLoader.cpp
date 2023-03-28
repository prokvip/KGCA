#define  _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "TFbxImporter.h"
namespace TBasisFBX
{
	FbxVector4 TFbxImporter::ReadTangent(const FbxMesh* mesh,
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

	// ���� �븻�� �д� �Լ� 
	FbxVector4 TFbxImporter::ReadNormal(const FbxMesh* mesh,
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
	FbxVector4 TFbxImporter::ReadNormal(const FbxMesh* mesh,
		int controlPointIndex,
		int vertexCounter)
	{
		if (mesh->GetElementNormalCount() < 1) {}

		const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
		// �븻 ȹ�� 
		FbxVector4 result;
		// �븻 ���͸� ������ ���� 
		switch (vertexNormal->GetMappingMode()) 	// ���� ��� 
		{
			// ������ ���� 1���� ���� ��ǥ�� �ִ�.
		case FbxGeometryElement::eByControlPoint:
		{
			// control point mapping 
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
				result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
				result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
			} break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
				// �ε����� ���´�. 
				result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
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
				result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
				result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
				result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				// �ε����� ���´�. 
				result[0] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result[1] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result[2] = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}break;
			}
		}break;
		}
		return result;
	}

	FbxColor TFbxImporter::ReadColor(const FbxMesh* mesh,
		DWORD dwVertexColorCount,
		FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex)
	{
		FbxColor Value(1, 1, 1, 1);
		if (dwVertexColorCount > 0 && pVertexColorSet != NULL)
		{
			// Crack apart the FBX dereferencing system for Color coordinates		
			switch (pVertexColorSet->GetMappingMode())
			{
			case FbxLayerElement::eByControlPoint:
				switch (pVertexColorSet->GetReferenceMode())
				{
				case FbxLayerElement::eDirect:
				{
					Value = pVertexColorSet->GetDirectArray().GetAt(dwDCCIndex);
				}break;
				case FbxLayerElement::eIndexToDirect:
				{
					int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
					Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
				}break;
				}
			case FbxLayerElement::eByPolygonVertex:
				switch (pVertexColorSet->GetReferenceMode())
				{
				case FbxLayerElement::eDirect:
				{
					int iColorIndex = dwVertexIndex;
					Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
				}break;
				case FbxLayerElement::eIndexToDirect:
				{
					int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwVertexIndex);
					Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
				}break;
				}
				break;
			}
		}
		return Value;
	}

	std::string TFbxImporter::ParseMaterial(FbxSurfaceMaterial* pMtrl)
	{
		std::string name = pMtrl->GetName();
		auto Property = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (Property.IsValid())
		{
			const FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
			if (tex != nullptr)
			{
				const CHAR* szFileName = tex->GetFileName();
				CHAR Drive[MAX_PATH];
				CHAR Dir[MAX_PATH];
				CHAR FName[MAX_PATH];
				CHAR Ext[MAX_PATH];
				_splitpath_s(szFileName, Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
				std::string texName = FName;
				std::string ext = Ext;
				if (ext == ".tga" || ext == ".TGA")
				{
					ext.clear();
					ext = ".dds";
				}
				texName += ext;
				return texName;
			}
		}
		return std::string("");
	}
	void TFbxImporter::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,
		int vertexIndex, int uvIndex, FbxVector2& uv)
	{
		FbxLayerElementUV* pFbxLayerElementUV = pUVSet;
		if (pFbxLayerElementUV == nullptr) {
			return;
		}

		switch (pFbxLayerElementUV->GetMappingMode())
		{
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
			case FbxLayerElementUV::eDirect:
			{
				FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(vertexIndex);
				uv.mData[0] = fbxUv.mData[0];
				uv.mData[1] = fbxUv.mData[1];
				break;
			}
			case FbxLayerElementUV::eIndexToDirect:
			{
				int id = pFbxLayerElementUV->GetIndexArray().GetAt(vertexIndex);
				FbxVector2 fbxUv = pFbxLayerElementUV->GetDirectArray().GetAt(id);
				uv.mData[0] = fbxUv.mData[0];
				uv.mData[1] = fbxUv.mData[1];
				break;
			}
			}
			break;
		}
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pFbxLayerElementUV->GetReferenceMode())
			{
				// Always enters this part for the example model
			case FbxLayerElementUV::eDirect: // fbx 5.0 ������ ��
			case FbxLayerElementUV::eIndexToDirect:
			{
				uv.mData[0] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[0];
				uv.mData[1] = pFbxLayerElementUV->GetDirectArray().GetAt(uvIndex).mData[1];
				break;
			}
			}
			break;
		}
		}
	}

	int TFbxImporter::GetSubMaterialIndex(int iPoly,
		FbxLayerElementMaterial* pMaterialSetList)
	{
		// ���ι��
		//eNone,
		//eByControlPoint,  // ������
		//eByPolygonVertex, //  
		//eByPolygon, // �����︶�� �ٸ��� �ִ�.
		//eAllSame - ��üǥ�鿡 1���� ������ǥ�� �ִ�.
		int iSubMtrl = 0;
		if (pMaterialSetList != nullptr)
		{
			switch (pMaterialSetList->GetMappingMode())
			{
			case FbxLayerElement::eByPolygon:
			{
				// ���� ������ �迭�� ����Ǵ� ���
				switch (pMaterialSetList->GetReferenceMode())
				{
				case FbxLayerElement::eIndex:
				{
					iSubMtrl = iPoly;
				}break;
				case FbxLayerElement::eIndexToDirect:
				{
					iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
				}break;
				}
			}
			default:
			{
				break;
			}
			}
		}
		return iSubMtrl;
	}
};
