#include "TFbxObj.h"
TFbxMaterial* TFbxObj::ParseMaterial(FbxSurfaceMaterial* pFbxMaterial)
{
	if (!pFbxMaterial)
		return nullptr;

	bool bRenameMaterial = false;
	TFbxMaterial* pMaterial = new TFbxMaterial();
	pMaterial->m_Name = pFbxMaterial->GetName();

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
	{ FbxSurfaceMaterial::sAmbient,            "AOTexture",	                   PPO_Nothing,                ExportMaterialParameter::EMPF_AOMAP },
	{ FbxSurfaceMaterial::sBump,               "NormalMapTexture",             PPO_Nothing,                0 /*ExportMaterialParameter::EMPF_BUMPMAP*/ },
	{ FbxSurfaceMaterial::sNormalMap,          "NormalMapTexture",             PPO_Nothing,                ExportMaterialParameter::EMPF_NORMALMAP },
	{ FbxSurfaceMaterial::sSpecular,           "SpecularMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
	{ FbxSurfaceMaterial::sBump,           "sBumpMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
	{ FbxSurfaceMaterial::sNormalMap,           "NormalMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
	{ FbxSurfaceMaterial::sReflection,           "ReflectionMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
	{ FbxSurfaceMaterial::sEmissive,           "EmissiveMapTexture",           PPO_Nothing,                0 },
	};

	for (DWORD dwExtractionIndex = 0; dwExtractionIndex < ARRAYSIZE(ExtractionList); ++dwExtractionIndex)
	{
		const TextureParameterExtraction& tpe = ExtractionList[dwExtractionIndex];

		auto Property = pFbxMaterial->FindProperty(tpe.strFbxPropertyName);
		if (!Property.IsValid())
			continue;

		bool bFound = ExtractTextures(Property, tpe.strParameterName, pMaterial, tpe.dwParameterFlags);
		if (bFound)
		{
			if (tpe.dwPostOperations & PPO_TransparentMaterial)
			{
				pMaterial->SetTransparent(true);
			}
		}
	}

	//FixupGenericMaterial(pMaterial);

	return pMaterial;
}