#pragma once
#include "TFbxSkeleton.h"

enum ExportMaterialParameterType
{
	MPT_STRING = 0,
	MPT_BOOL,
	MPT_INTEGER,
	MPT_FLOAT,
	MPT_FLOAT2,
	MPT_FLOAT3,
	MPT_FLOAT4,
	MPT_TEXTURE2D,
	MPT_TEXTURECUBE,
	MPT_TEXTUREVOLUME
};
struct ExportAttribute
{
	enum AttributeType
	{
		AT_UNKNOWN = 0,
		AT_STRING,
		AT_INT,
		AT_FLOAT,
		AT_VEC4
	};
	ExportAttribute()
	{
		ZeroMemory(this, sizeof(ExportAttribute));
	}
	std::string    Name;
	AttributeType   Type;
	//ExportString    strValue;
	INT             iValue;
	FLOAT           fValue;
	TVector4     vecValue;
};

class ExportMaterialParameter
{
public:
	enum ExportMaterialParameterFlags
	{
		EMPF_NONE = 0,
		EMPF_BUMPMAP = 1,
		EMPF_DIFFUSEMAP = 2,
		EMPF_NORMALMAP = 4,
		EMPF_SPECULARMAP = 8,
		EMPF_ALPHACHANNEL = 16,
		EMPF_AOMAP = 32,
	};
	ExportMaterialParameter()
	{
		ZeroMemory(this, sizeof(ExportMaterialParameter));
	}
	string                    Name;
	ExportMaterialParameterType     ParamType;
	string                    Hint;
	BOOL                            bInstanceParam;
	string                    ValueString;
	FLOAT                           ValueFloat[16];
	INT                             ValueInt;
	DWORD                           Flags;
};

typedef std::list< ExportAttribute* > ExportAttributeList;
typedef std::vector<ExportMaterialParameter> MaterialParameterList;

class TFbxMaterial
{
public:
	string        m_Name;
	ExportAttributeList m_Attributes;
	MaterialParameterList               m_Parameters;
	BOOL		m_bTransparent;
public:
	VOID AddParameter(CONST ExportMaterialParameter& Param) { m_Parameters.push_back(Param); }
	BOOL IsTransparent() const { return m_bTransparent; }
	VOID SetTransparent(BOOL bTransparent) { m_bTransparent = bTransparent; }
public:
	TFbxMaterial();
	TFbxMaterial(string name);
	~TFbxMaterial();
};

